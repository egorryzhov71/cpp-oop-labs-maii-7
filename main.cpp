#include "/workspaces/c++/cpp-oop-labs-maii-7/include/NPCFactory.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Observer.h"
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <optional>
#include <array>
#include <cstdlib> 
#include <ctime>    
#include <atomic>

using namespace std::chrono_literals;

std::atomic<bool> game_running{true};

struct FightEvent {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class FightManager {
private:
    std::queue<FightEvent> events;
    std::mutex mtx;
    std::atomic<bool> running{true};
    
    FightManager() {}
    
public:
    static FightManager& get() {
        static FightManager instance;
        return instance;
    }

    void add_event(FightEvent&& event) {
        std::lock_guard<std::mutex> lck(mtx);
        events.push(event);
    }
    
    void stop() {
        running = false;
    }

    void operator()() {
        while (running && game_running) {
            std::optional<FightEvent> event;
            
            {
                std::lock_guard<std::mutex> lck(mtx);
                if (!events.empty()) {
                    event = events.front();  
                    events.pop();
                }
            }

            if (event) {
                if (event->attacker->is_alive() &&     
                    event->defender->is_alive()) {      
                    int kill_distance = event->attacker->get_kill_distance();
                    if (event->attacker->is_close(event->defender, kill_distance)) {
                        if (event->defender->accept(event->attacker)) {
                            event->defender->must_die();
                        }
                    }
                }
            }
            
            std::this_thread::sleep_for(100ms);
        }
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    set_t array; 
    const int MAX_X{100};
    const int MAX_Y{100};

    std::cout << "Generating 50 NPCs..." << std::endl;
    for (size_t i = 0; i < 50; ++i) {
        NPC::Type type;
        int rand_type = std::rand() % 3;
        switch (rand_type) {
            case 0: type = NPC::Type::Knight; break;
            case 1: type = NPC::Type::Orc; break;
            case 2: type = NPC::Type::Bear; break;
            default: type = NPC::Type::Knight;
        }
        
        auto npc = NPCFactory::factory(type,
                                       std::rand() % MAX_X,
                                       std::rand() % MAX_Y);
        if (npc) {
            array.insert(npc);
        }
    }

    std::cout << "\nStarting with " << array.size() << " NPCs:" << std::endl;
    std::cout << "Knight: move=30, kill=10" << std::endl;
    std::cout << "Orc: move=20, kill=10" << std::endl;
    std::cout << "Bear: move=5, kill=10" << std::endl;
    std::cout << "=================================" << std::endl;

    auto& fight_manager = FightManager::get();
    std::thread fight_thread(std::ref(fight_manager));

   std::thread move_thread([&array, MAX_X, MAX_Y, &fight_manager]() {
        while (game_running) {
            for (const std::shared_ptr<NPC>& npc : array) {
                if (npc->is_alive()) {

                    npc->move(MAX_X, MAX_Y);
                }
            }

            for (const std::shared_ptr<NPC>& npc : array) {
                for (const std::shared_ptr<NPC>& other : array) {
                    if ((other != npc) && 
                        npc->is_alive() && 
                        other->is_alive()) {
                        int kill_distance = npc->get_kill_distance();
                        if (npc->is_close(other, kill_distance)) {
                            fight_manager.add_event({npc, other});
                        }
                    }
                }
            }
            std::this_thread::sleep_for(100ms); 
        }
    });

    const int grid{20}, step_x{MAX_X / grid}, step_y{MAX_Y / grid};
    std::array<char, grid * grid> fields{0};
    
    auto start_time = std::chrono::steady_clock::now();
    int seconds_passed = 0;
    
    while (game_running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        seconds_passed = static_cast<int>(elapsed.count());
        
        if (seconds_passed >= 30) {
            game_running = false;
            break;
        }
        
        fields.fill(0);
        
        for (const std::shared_ptr<NPC>& npc : array) {
            const auto [x, y] = npc->position();
            int i = x / step_x;
            int j = y / step_y;

            if (i >= 0 && i < grid && j >= 0 && j < grid) {
                if (npc->is_alive()) {
                    switch (npc->get_type()) {
                        case NPC::Type::Knight:
                            fields[i + grid * j] = 'K';
                            break;
                        case NPC::Type::Orc:
                            fields[i + grid * j] = 'O';
                            break;
                        case NPC::Type::Bear:
                            fields[i + grid * j] = 'B';
                            break;
                        default:
                            break;
                    }
                } else {
                    fields[i + grid * j] = '.';
                }
            }
        }
        
        system("clear");

        std::cout << "=== NPC Simulation ===" << std::endl;
        std::cout << "Time: " << seconds_passed << "/30 seconds" << std::endl;
        std::cout << "K - Knight, O - Orc, B - Bear, . - Dead" << std::endl << std::endl;
        
        for (int j = 0; j < grid; ++j) {
            for (int i = 0; i < grid; ++i) {
                char c = fields[i + j * grid];
                if (c != 0) {
                    std::cout << "[" << c << "]";
                } else {
                    std::cout << "[ ]";
                }
            }
            std::cout << std::endl;
        }
        
        int alive_count = 0;
        for (const auto& npc : array) {
            if (npc->is_alive()) alive_count++;
        }
        std::cout << "\nAlive NPCs: " << alive_count << "/" << array.size() << std::endl;
        
        std::this_thread::sleep_for(1s);
    }

    fight_manager.stop();
    
    if (move_thread.joinable()) move_thread.join();
    if (fight_thread.joinable()) fight_thread.join();

    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "Simulation stopped after 30 seconds" << std::endl;
    std::cout << "\n=== SURVIVORS ===" << std::endl;
    
    int survivor_count = 0;
    for (const auto& npc : array) {
        if (npc->is_alive()) {
            survivor_count++;
            npc->print();
        }
    }
    
    if (survivor_count == 0) {
        std::cout << "No survivors..." << std::endl;
    } else {
        std::cout << "\nTotal survivors: " << survivor_count << std::endl;
    }
    
    std::cout << "\n=== STATISTICS ===" << std::endl;
    std::cout << "Initial NPCs: " << array.size() << std::endl;
    std::cout << "Survivors: " << survivor_count << std::endl;
    std::cout << "Killed: " << array.size() - survivor_count << std::endl;

    return 0;
}