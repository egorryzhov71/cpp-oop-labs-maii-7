#include "/workspaces/C++/balagur_project_filled/include/Knight.h"
#include <iostream>
#include <memory>

Knight::Knight(int x, int y, const std::string& name) 
    : NPC(Type::Knight, x, y, name) {}

Knight::Knight(std::istream &is) : NPC(Type::Knight, is) {}

bool Knight::accept(std::shared_ptr<NPC> attacker) {
    return attacker->fight(std::static_pointer_cast<Knight>(shared_from_this()));
}

bool Knight::fight(std::shared_ptr<Knight> other) {
    fight_notify(other, false);
    return false;
}

bool Knight::fight(std::shared_ptr<Orc> other) {
    fight_notify(other, true);
    return true;
}

bool Knight::fight(std::shared_ptr<Bear> other) {
    fight_notify(other, false);
    return false;
}

void Knight::print() {
    std::cout << "Рыцарь '" << name << "' [" << x << "," << y << "]" << std::endl;
}

void Knight::save(std::ostream &os) {
    os << static_cast<int>(Type::Knight) << " ";
    NPC::save(os);
}