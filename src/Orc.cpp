#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Orc.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Bear.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Knight.h"
#include <iostream>

Orc::Orc(int x, int y, const std::string& name) 
    : NPC(Type::Orc, x, y, name) {}

Orc::Orc(std::istream &is) : NPC(Type::Orc, is) {}

bool Orc::accept(std::shared_ptr<NPC> attacker) {
    return attacker->fight(std::static_pointer_cast<Orc>(shared_from_this()));
}

bool Orc::fight(std::shared_ptr<Knight> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

bool Orc::fight(std::shared_ptr<Orc> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

bool Orc::fight(std::shared_ptr<Bear> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

void Orc::print() {
    std::cout << "Орк '" << name << "' [" << x << "," << y << "] "
              << "АТК:" << attack_power << " ЗЩТ:" << defense_power << std::endl;
}

void Orc::save(std::ostream &os) {
    os << static_cast<int>(Type::Orc) << " ";
    NPC::save(os);
}