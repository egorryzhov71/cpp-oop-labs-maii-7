#include "/workspaces/C++/balagur_project_filled/include/NPC.h"
#include "/workspaces/C++/balagur_project_filled/include/Observer.h"
#include <iostream>
#include <sstream>
#include <memory>

NPC::NPC(Type t, int _x, int _y, const std::string& _name) 
    : type(t), x(_x), y(_y), name(_name) {}

NPC::NPC(Type t, std::istream &is) : type(t) {
    is >> x >> y >> std::ws;
    std::getline(is, name);
    if (!name.empty() && name[0] == ' ') {
        name = name.substr(1);
    }
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer) {
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win) {
    for (auto& observer : observers) {
        observer->on_fight(shared_from_this(), defender, win);
    }
}

void NPC::save(std::ostream &os) {
    os << x << " " << y << " " << name;
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    if (!other) return false;
    double dx = x - other->x;
    double dy = y - other->y;
    return (dx * dx + dy * dy) <= (distance * distance);
}