#include "/workspaces/C++/LABA7/include/ThreadedGame.h"
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> exit_flag(false);

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nInterrupt received, stopping game..." << std::endl;
        exit_flag.store(true);
    }
}

int main() {
    std::signal(SIGINT, signal_handler);
    
    std::cout << "==========================================" << std::endl;
    std::cout << "    MULTI-THREADED DUNGEON GAME (Lab 7)   " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "\nGame Features:" << std::endl;
    std::cout << "• 3 NPC types: Knight, Orc, Bear" << std::endl;
    std::cout << "• Kill cycle: Knight → Orc → Bear → Knight" << std::endl;
    std::cout << "• Move distances: Knight(30), Orc(20), Bear(5)" << std::endl;
    std::cout << "• Kill distance: 10 for all types" << std::endl;
    std::cout << "• Battle: 6-sided dice roll (attack vs defense)" << std::endl;
    std::cout << "• Map size: 100x100" << std::endl;
    std::cout << "• Game duration: 30 seconds" << std::endl;
    std::cout << "• Initial NPCs: 50" << std::endl;
    std::cout << "\nThread Architecture:" << std::endl;
    std::cout << "1. Movement thread (lambda)" << std::endl;
    std::cout << "2. Fight thread (lambda)" << std::endl;
    std::cout << "3. Main thread (map printing every second)" << std::endl;
    std::cout << "==========================================\n" << std::endl;
    
    try {
        ThreadedGame game;
        
        std::cout << "Initializing game with 50 NPCs..." << std::endl;
        game.initialize();
        
        std::cout << "\nStarting game in 2 seconds..." << std::endl;
        std::cout << "Press Ctrl+C to stop early.\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        game.run();
        
        std::cout << "\n==========================================" << std::endl;
        std::cout << "          PROGRAM FINISHED            " << std::endl;
        std::cout << "==========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}