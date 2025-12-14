#include "/workspaces/C++/balagur_project_filled/include/Orc.h"
#include "/workspaces/C++/balagur_project_filled/include/Bear.h"
#include "/workspaces/C++/balagur_project_filled/include/Knight.h"
#include <iostream>

Orc::Orc(int x, int y, const std::string& name) 
    : NPC(Type::Orc, x, y, name) {}

Orc::Orc(std::istream &is) : NPC(Type::Orc, is) {}

bool Orc::accept(std::shared_ptr<NPC> attacker) {
    return attacker->fight(std::static_pointer_cast<Orc>(shared_from_this()));
}

bool Orc::fight(std::shared_ptr<Knight> other) {
    fight_notify(other, false);
    return false;
}

bool Orc::fight(std::shared_ptr<Orc> other) {
    fight_notify(other, false);
    return false;
}

bool Orc::fight(std::shared_ptr<Bear> other) {
    fight_notify(other, true);
    return true;
}

void Orc::print() {
    std::cout << "Орк '" << name << "' [" << x << "," << y << "]" << std::endl;
}

void Orc::save(std::ostream &os) {
    os << static_cast<int>(Type::Orc) << " ";
    NPC::save(os);
}