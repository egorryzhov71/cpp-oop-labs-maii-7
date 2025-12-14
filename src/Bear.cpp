#include "/workspaces/C++/balagur_project_filled/include/Bear.h"
#include <iostream>

Bear::Bear(int x, int y, const std::string& name) 
    : NPC(Type::Bear, x, y, name) {}

Bear::Bear(std::istream &is) : NPC(Type::Bear, is) {}

bool Bear::accept(std::shared_ptr<NPC> attacker) {
    return attacker->fight(std::static_pointer_cast<Bear>(shared_from_this()));
}

bool Bear::fight(std::shared_ptr<Knight> other) {
    std::cout << name << " (Bear) kills " << other->get_name() << " (Knight)" << std::endl;
    return true;
}

bool Bear::fight(std::shared_ptr<Orc> other) {
    std::cout << name << " (Bear) loses to " << other->get_name() << " (Orc)" << std::endl;
    return false;
}

bool Bear::fight(std::shared_ptr<Bear> other) {
    std::cout << name << " (Bear) ignores " << other->get_name() << " (Bear)" << std::endl;
    return false;
}

void Bear::print() {
    std::cout << "Bear " << name << " at (" << x << ", " << y << ")" << std::endl;
}

void Bear::save(std::ostream &os) {
    os << static_cast<int>(type) << " ";
    NPC::save(os);
}