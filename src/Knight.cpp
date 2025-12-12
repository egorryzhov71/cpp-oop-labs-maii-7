#include "/workspaces/C++/LABA7/include/Knight.h"

Knight::Knight(const std::string& name_, double x_, double y_) 
    : NPC(name_, x_, y_) {}

std::string Knight::type() const { return "Knight"; }
int Knight::move_distance() const { return 30; }    
int Knight::kill_distance() const { return 10; }   

bool Knight::kills(const NPC& other) const {
    return other.type() == "Orc";
}