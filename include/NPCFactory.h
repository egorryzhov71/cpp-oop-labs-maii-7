#pragma once
#include "NPC.h"
#include "Knight.h"
#include "Orc.h"
#include "Bear.h"
#include "Observer.h"
#include <memory>
#include <random>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

class NPCFactory {
    std::shared_ptr<IFightObserver> console_observer;
    std::shared_ptr<IFightObserver> file_observer;
    std::set<std::string> used_names;
    
    std::string generate_name() {
        static std::vector<std::string> names = {"Гром", "Тень", "Коготь", "Ярость", "Сталь"};
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return names[std::uniform_int_distribution<>(0, names.size()-1)(gen)];
    }
    
    std::string generate_unique_name() {
        for (int i = 0; i < 100; ++i) {
            std::string name = generate_name();
            if (used_names.find(name) == used_names.end()) return name;
        }
        return generate_name() + "_" + std::to_string(used_names.size());
    }
    
public:
    NPCFactory() {
        console_observer = TextObserver::get(); 
        file_observer = std::make_shared<FileObserver>();
    }
    std::shared_ptr<NPC> create(NPC::Type type, int x, int y, const std::string& name = "") {
        if (x < 0 || x > 100 || y < 0 || y > 100) return nullptr;
        
        std::string npc_name = name.empty() ? generate_unique_name() : name;
        if (used_names.find(npc_name) != used_names.end()) return nullptr;
        
        std::shared_ptr<NPC> npc;
        switch(type) {
            case NPC::Type::Knight: npc = std::make_shared<Knight>(x, y, npc_name); break;
            case NPC::Type::Orc: npc = std::make_shared<Orc>(x, y, npc_name); break;
            case NPC::Type::Bear: npc = std::make_shared<Bear>(x, y, npc_name); break;
            default: return nullptr;
        }
        
        used_names.insert(npc_name);
        npc->subscribe(console_observer);
        npc->subscribe(file_observer);
        return npc;
    }
    
    std::shared_ptr<NPC> load(std::istream &is) {
        int type;
        if (!(is >> type)) return nullptr;
        
        std::shared_ptr<NPC> npc;
        switch(static_cast<NPC::Type>(type)) {
            case NPC::Type::Knight: npc = std::make_shared<Knight>(is); break;
            case NPC::Type::Orc: npc = std::make_shared<Orc>(is); break;
            case NPC::Type::Bear: npc = std::make_shared<Bear>(is); break;
            default: return nullptr;
        }
        
        used_names.insert(npc->get_name());
        npc->subscribe(console_observer);
        npc->subscribe(file_observer);
        return npc;
    }
    
    static std::shared_ptr<NPC> factory(std::istream &is) {
        static NPCFactory instance;
        return instance.load(is);
    }
    
    static std::shared_ptr<NPC> factory(NPC::Type type, int x, int y, const std::string& name = "") {
        static NPCFactory instance;
        return instance.create(type, x, y, name);
    }
    
    static void save(const std::set<std::shared_ptr<NPC>>& array, const std::string &filename) {
        std::ofstream fs(filename);
        fs << array.size() << std::endl;
        for (auto &n : array) {
            n->save(fs);
        }
        fs.flush();
        fs.close();
    }
    
    static std::set<std::shared_ptr<NPC>> load_set(const std::string &filename) {
        std::set<std::shared_ptr<NPC>> result;
        std::ifstream is(filename);
        if (is.good() && is.is_open()) {
            int count;
            is >> count;
            for (int i = 0; i < count; ++i) {
                if (auto npc = factory(is)) {
                    result.insert(npc);
                }
            }
            is.close();
        } else {
            std::cerr << "Error: " << std::strerror(errno) << std::endl;
        }
        return result;
    }
};

using set_t = std::set<std::shared_ptr<NPC>>;