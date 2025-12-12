#ifndef THREADEDGAME_H
#define THREADEDGAME_H

#include "NPC.h"
#include "NPCFactory.h"
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>

struct FightTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class ThreadedGame {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::queue<FightTask> fight_queue;
    
    std::thread movement_thread;
    std::thread fight_thread;
    
    mutable std::shared_mutex npcs_mutex;
    mutable std::mutex queue_mutex;
    mutable std::mutex cout_mutex;
    std::condition_variable queue_cv;
    
    std::atomic<bool> running;
    std::atomic<int> time_elapsed;
    
    const int MAP_WIDTH = 100;
    const int MAP_HEIGHT = 100;
    const int GAME_DURATION = 30;     
    const int TOTAL_NPCS = 50;        
    const int PRINT_INTERVAL = 1;     
    
    void cleanup_dead_npcs();
    
public:
    ThreadedGame();
    ~ThreadedGame();
    
    void initialize();
    void run();                      
    void stop();
    
    void print_survivors() const;
    void print_map() const;
    
    bool is_running() const { return running.load(); }
    int get_time_elapsed() const { return time_elapsed.load(); }
    
private:
    void process_movements();
    void process_fights();
};

#endif 