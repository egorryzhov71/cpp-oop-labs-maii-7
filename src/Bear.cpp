#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Bear.h"
#include <iostream>

Bear::Bear(int x, int y, const std::string& name) 
    : NPC(Type::Bear, x, y, name) {}

Bear::Bear(std::istream &is) : NPC(Type::Bear, is) {}

bool Bear::accept(std::shared_ptr<NPC> attacker) {
    return attacker->fight(std::static_pointer_cast<Bear>(shared_from_this()));
}

bool Bear::fight(std::shared_ptr<Knight> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

bool Bear::fight(std::shared_ptr<Orc> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

bool Bear::fight(std::shared_ptr<Bear> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

void Bear::print() {
    std::cout << "Медведь '" << name << "' [" << x << "," << y << "] "
              << "АТК:" << attack_power << " ЗЩТ:" << defense_power << std::endl;
}

void Bear::save(std::ostream &os) {
    os << static_cast<int>(type) << " ";
    NPC::save(os);
}