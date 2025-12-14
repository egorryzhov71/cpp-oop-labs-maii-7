#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <cmath>

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
    Type type;
    int x{0};
    int y{0};
    std::string name;
    std::vector<std::shared_ptr<IFightObserver>> observers;

public:
    NPC(Type t, int _x, int _y, const std::string& _name);
    NPC(Type t, std::istream &is);
    virtual ~NPC() = default;

    void subscribe(std::shared_ptr<IFightObserver> observer);
    void fight_notify(const std::shared_ptr<NPC> defender, bool win);
    
    virtual bool accept(std::shared_ptr<NPC> attacker) = 0;
    
    virtual bool fight(std::shared_ptr<Knight> other) = 0;
    virtual bool fight(std::shared_ptr<Orc> other) = 0;
    virtual bool fight(std::shared_ptr<Bear> other) = 0;
    
    virtual void print() = 0;
    virtual void save(std::ostream &os);
    bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;
    
    std::string get_name() const { return name; }
};

using set_t = std::set<std::shared_ptr<NPC>>;