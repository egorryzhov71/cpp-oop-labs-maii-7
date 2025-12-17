#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <set>
#include <cmath>
#include <shared_mutex>


class IFightObserver;
class Knight;
class Orc;
class Bear;

class NPC : public std::enable_shared_from_this<NPC> {
public:
    enum class Type {
        Unknown = 0,
        Knight = 1,
        Orc = 2,
        Bear = 3
    };

protected:
    std::mutex mtx;

    Type type;
    int x{0};
    int y{0};
    bool alive{true};
    std::string name;
    std::vector<std::shared_ptr<IFightObserver>> observers;
    
    int attack_power{0};
    int defense_power{0};

public:
    NPC(Type t, int _x, int _y, const std::string& _name);
    NPC(Type t, std::istream &is);
    virtual ~NPC() = default;

    void roll_dice() {
        attack_power = 1 + std::rand() % 6;
        defense_power = 1 + std::rand() % 6;
    }

    void subscribe(std::shared_ptr<IFightObserver> observer);
    void fight_notify(const std::shared_ptr<NPC> defender, bool win);
    
    virtual bool accept(std::shared_ptr<NPC> attacker) = 0;
    
    virtual bool fight(std::shared_ptr<Knight> other) = 0;
    virtual bool fight(std::shared_ptr<Orc> other) = 0;
    virtual bool fight(std::shared_ptr<Bear> other) = 0;
    
    virtual void print() = 0;
    std::pair<int, int> position() const;
    Type get_type() const;
    

    virtual int get_move_distance() const = 0;    
    virtual int get_kill_distance() const = 0;    
    
    void move(int max_x, int max_y); 
    virtual void save(std::ostream &os);
    bool is_close(const std::shared_ptr<NPC> &other, int distance) const;

    bool is_alive() const;
    void must_die();
    
    int get_attack_power() const { return attack_power; }
    int get_defense_power() const { return defense_power; }
    
    std::string get_name() const { return name; }
};

using set_t = std::set<std::shared_ptr<NPC>>;