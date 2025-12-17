#include "/workspaces/c++/cpp-oop-labs-maii-7/include/NPC.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Observer.h"
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

NPC::Type NPC::get_type() const
{
    return type;
}

std::pair<int, int> NPC::position() const
{
    return {x, y};
}

// НОВЫЙ МЕТОД ДВИЖЕНИЯ с учетом таблицы
void NPC::move(int max_x, int max_y) {
    std::lock_guard<std::mutex> lck(mtx);
    
    if (!alive) return;
    
    roll_dice();
    
    int move_dist = get_move_distance();
    int shift_x = (std::rand() % (2 * move_dist + 1)) - move_dist;
    int shift_y = (std::rand() % (2 * move_dist + 1)) - move_dist;
    
    if ((x + shift_x >= 0) && (x + shift_x <= max_x))
        x += shift_x;
    if ((y + shift_y >= 0) && (y + shift_y <= max_y))
        y += shift_y;
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, int distance) const {
    if (!other || !other->is_alive()) return false;
    double dx = x - other->x;
    double dy = y - other->y;
    return (dx * dx + dy * dy) <= (distance * distance);
}

bool NPC::is_alive() const
{
    return alive;
}

void NPC::must_die()
{
    std::lock_guard<std::mutex> lck(mtx);
    alive = false;
}