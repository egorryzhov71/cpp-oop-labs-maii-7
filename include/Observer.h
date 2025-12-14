#pragma once
#include "NPC.h"
#include <fstream>
#include <iostream>
#include <memory>

class IFightObserver {
public:
    virtual ~IFightObserver() = default;
    virtual void on_fight(const std::shared_ptr<NPC> attacker, 
                         const std::shared_ptr<NPC> defender, 
                         bool win) = 0;
};

class ConsoleObserver : public IFightObserver {
public:
    void on_fight(const std::shared_ptr<NPC> attacker, 
                 const std::shared_ptr<NPC> defender, 
                 bool win) override {
        if (win) {
            std::cout << attacker->get_name() << " убил " << defender->get_name() << std::endl;
        }
    }
};

class FileObserver : public IFightObserver {
    std::ofstream log_file;
public:
    FileObserver() { 
        log_file.open("log.txt", std::ios::app); 
    }
    ~FileObserver() { 
        if (log_file.is_open()) log_file.close(); 
    }
    
    void on_fight(const std::shared_ptr<NPC> attacker, 
                 const std::shared_ptr<NPC> defender, 
                 bool win) override {
        if (win && log_file.is_open()) {
            log_file << attacker->get_name() << " убил " << defender->get_name() << std::endl;
        }
    }
};