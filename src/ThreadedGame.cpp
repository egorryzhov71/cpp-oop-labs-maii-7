#include "/workspaces/C++/LABA7/include/ThreadedGame.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <sstream>

ThreadedGame::ThreadedGame() 
    : running(false), time_elapsed(0) {
    npcs.reserve(TOTAL_NPCS * 2);
}

ThreadedGame::~ThreadedGame() {
    stop();
}

void ThreadedGame::initialize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> type_dist(0, 2);
    std::uniform_real_distribution<> pos_dist(0, MAP_WIDTH);
    
    const char* types[] = {"Orc", "Knight", "Bear"};
    
    for (int i = 0; i < TOTAL_NPCS; ++i) {
        int type_idx = type_dist(gen);
        std::string name = std::string(types[type_idx]) + "_" + std::to_string(i);
        
        try {
            auto npc = NPCFactory::create(types[type_idx], name, 
                                         pos_dist(gen), pos_dist(gen));
            npcs.push_back(npc);
        } catch (const std::exception& e) {
            std::lock_guard lock(cout_mutex);
            std::cerr << "Error creating NPC: " << e.what() << std::endl;
        }
    }
    
    std::lock_guard lock(cout_mutex);
    std::cout << "=== Game Initialized ===" << std::endl;
    std::cout << "Created " << npcs.size() << " NPCs" << std::endl;
    std::cout << "Map: " << MAP_WIDTH << "x" << MAP_HEIGHT << std::endl;
    std::cout << "Duration: " << GAME_DURATION << " seconds" << std::endl;
    std::cout << "=======================\n" << std::endl;
}

void ThreadedGame::process_movements() {
    while (running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        

        {
            std::lock_guard lock(npcs_mutex);
            for (auto& npc : npcs) {
                if (npc->alive) {
                    npc->move_randomly(MAP_WIDTH, MAP_HEIGHT);
                }
            }
        }

        {
            std::lock_guard lock(npcs_mutex);
            for (size_t i = 0; i < npcs.size(); ++i) {
                auto& a = npcs[i];
                if (!a->alive) continue;
                
                for (size_t j = i + 1; j < npcs.size(); ++j) {
                    auto& b = npcs[j];
                    if (!b->alive) continue;
                    
                    double dist = a->distance_to(*b);
                    if (dist <= a->kill_distance() || dist <= b->kill_distance()) {
                        FightTask task;
                        
                        if (a->kills(*b)) {
                            task.attacker = a;
                            task.defender = b;
                        } else if (b->kills(*a)) {
                            task.attacker = b;
                            task.defender = a;
                        }
                        
                        if (task.attacker && task.defender) {
                            std::lock_guard q_lock(queue_mutex);
                            fight_queue.push(task);
                            queue_cv.notify_one();
                        }
                    }
                }
            }
        }
        
        cleanup_dead_npcs();
    }
}

void ThreadedGame::process_fights() {
    while (running.load()) {
        FightTask task;
        
        {
            std::unique_lock lock(queue_mutex);
            queue_cv.wait_for(lock, std::chrono::milliseconds(100),
                [this]() { return !fight_queue.empty() || !running.load(); });
            
            if (!running.load()) break;
            if (fight_queue.empty()) continue;
            
            task = fight_queue.front();
            fight_queue.pop();
        }
        
        if (task.attacker && task.defender &&
            task.attacker->alive && task.defender->alive) {
            
            bool killed = task.defender->fight(task.attacker.get());
            
            if (killed) {
                std::lock_guard lock(cout_mutex);
                std::cout << "[BATTLE] " << task.attacker->name << " (" 
                         << task.attacker->type() << ") killed " 
                         << task.defender->name << " (" 
                         << task.defender->type() << ")" << std::endl;
            }
        }
    }
}

void ThreadedGame::cleanup_dead_npcs() {
    std::lock_guard lock(npcs_mutex);
    npcs.erase(
        std::remove_if(npcs.begin(), npcs.end(),
            [](const std::shared_ptr<NPC>& npc) { return !npc->alive; }),
        npcs.end()
    );
}

void ThreadedGame::run() {
    if (running.load()) {
        return;
    }
    
    running.store(true);
    time_elapsed.store(0);
    
    auto movement_lambda = [this]() {
        this->process_movements();
    };
    
    auto fight_lambda = [this]() {
        this->process_fights();
    };
    
    std::cout << "Starting game with 2 worker threads (using lambdas)..." << std::endl;
    std::cout << "Main thread will print map every second.\n" << std::endl;
    
    movement_thread = std::thread(movement_lambda);
    fight_thread = std::thread(fight_lambda);
    
    auto last_print_time = std::chrono::steady_clock::now();
    
    while (running.load() && time_elapsed.load() < GAME_DURATION) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_print_time).count();
        
        if (elapsed >= PRINT_INTERVAL) {
            {
                std::lock_guard lock(cout_mutex);
                std::cout << "\n=== Time: " << time_elapsed.load() << "s ===" << std::endl;
                std::cout << "Alive NPCs: " << npcs.size() << std::endl;
                print_map();
            }
            
            time_elapsed++;
            last_print_time = now;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    stop();
    
    print_survivors();
}

void ThreadedGame::stop() {
    if (!running.load()) return;
    
    running.store(false);
    queue_cv.notify_all(); 
    
    if (movement_thread.joinable()) {
        movement_thread.join();
    }
    if (fight_thread.joinable()) {
        fight_thread.join();
    }
}

void ThreadedGame::print_map() const {
    const int GRID_WIDTH = 20;
    const int GRID_HEIGHT = 10;
    const float CELL_WIDTH = MAP_WIDTH / static_cast<float>(GRID_WIDTH);
    const float CELL_HEIGHT = MAP_HEIGHT / static_cast<float>(GRID_HEIGHT);

    std::vector<std::vector<char>> grid(GRID_HEIGHT, std::vector<char>(GRID_WIDTH, '.'));
    
    {
        std::shared_lock lock(npcs_mutex);
        for (const auto& npc : npcs) {
            if (npc->alive) {
                int cell_x = static_cast<int>(npc->x / CELL_WIDTH);
                int cell_y = static_cast<int>(npc->y / CELL_HEIGHT);

                cell_x = std::max(0, std::min(cell_x, GRID_WIDTH - 1));
                cell_y = std::max(0, std::min(cell_y, GRID_HEIGHT - 1));
                
                char symbol = '.';
                std::string type = npc->type();
                if (type == "Orc") symbol = 'O';
                else if (type == "Knight") symbol = 'K';
                else if (type == "Bear") symbol = 'B';
                
                if (grid[cell_y][cell_x] == '.') {
                    grid[cell_y][cell_x] = symbol;
                } else {
                    grid[cell_y][cell_x] = 'X';
                }
            }
        }
    }
    
    std::cout << "Map (" << MAP_WIDTH << "x" << MAP_HEIGHT << "):" << std::endl;
    std::cout << "O=Orc, K=Knight, B=Bear, X=Multiple, .=Empty" << std::endl;
    std::cout << "+" << std::string(GRID_WIDTH * 2 - 1, '-') << "+" << std::endl;
    
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        std::cout << "|";
        for (int x = 0; x < GRID_WIDTH; ++x) {
            std::cout << grid[y][x];
            if (x < GRID_WIDTH - 1) std::cout << " ";
        }
        std::cout << "|" << std::endl;
    }
    
    std::cout << "+" << std::string(GRID_WIDTH * 2 - 1, '-') << "+" << std::endl;
}

void ThreadedGame::print_survivors() const {
    std::shared_lock lock(npcs_mutex);
    std::lock_guard cout_lock(cout_mutex);
    
    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "Game duration: " << time_elapsed.load() << " seconds" << std::endl;
    std::cout << "Total survivors: " << npcs.size() << std::endl;
    
    if (!npcs.empty()) {
        std::cout << "\nSurvivor list:" << std::endl;
        std::cout << std::left << std::setw(15) << "Name" 
                  << std::setw(10) << "Type" 
                  << std::setw(20) << "Position" 
                  << std::endl;
        std::cout << std::string(45, '-') << std::endl;
        
        for (const auto& npc : npcs) {
            std::ostringstream pos;
            pos << "(" << std::fixed << std::setprecision(1) 
                << npc->x << ", " << npc->y << ")";
            
            std::cout << std::left << std::setw(15) << npc->name
                      << std::setw(10) << npc->type()
                      << std::setw(20) << pos.str()
                      << std::endl;
        }
    } else {
        std::cout << "\nNo survivors!" << std::endl;
    }
}