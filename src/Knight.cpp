#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Knight.h"
#include <iostream>
#include <memory>

Knight::Knight(int x, int y, const std::string& name) 
    : NPC(Type::Knight, x, y, name) {}

Knight::Knight(std::istream &is) : NPC(Type::Knight, is) {}

bool Knight::accept(std::shared_ptr<NPC> attacker) {
    return attacker->fight(std::static_pointer_cast<Knight>(shared_from_this()));
}

bool Knight::fight(std::shared_ptr<Knight> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->defense_power);
    fight_notify(other, win);
    return win;
}

bool Knight::fight(std::shared_ptr<Orc> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

bool Knight::fight(std::shared_ptr<Bear> other) {
    roll_dice();
    other->roll_dice();
    
    bool win = (attack_power > other->get_defense_power());
    fight_notify(other, win);
    return win;
}

void Knight::print() {
    std::cout << "Рыцарь '" << name << "' [" << x << "," << y << "] "
              << "АТК:" << attack_power << " ЗЩТ:" << defense_power << std::endl;
}

void Knight::save(std::ostream &os) {
    os << static_cast<int>(Type::Knight) << " ";
    NPC::save(os);
}