#pragma once
#include "NPC.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

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
            static std::mutex print_mutex;  // Встроенный static
            std::lock_guard<std::mutex> lck(print_mutex);
            std::cout << std::endl << "Murder --------" << std::endl;
            attacker->print();
            defender->print();
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
            static std::mutex file_mutex;  // Встроенный static
            std::lock_guard<std::mutex> lck(file_mutex);
            log_file << attacker->get_name() << " убил " << defender->get_name() << std::endl;
        }
    }
};

class TextObserver : public IFightObserver {
private:
    TextObserver() {};

public:
    static std::shared_ptr<IFightObserver> get() {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, 
                 const std::shared_ptr<NPC> defender, 
                 bool win) override {
        if (win) {
            std::cout << std::endl << "Murder --------" << std::endl;
            attacker->print();
            defender->print();
        }
    }
};