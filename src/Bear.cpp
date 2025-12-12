#include "/workspaces/C++/LABA7/include/Bear.h"

Bear::Bear(const std::string& name_, double x_, double y_) 
    : NPC(name_, x_, y_) {}

std::string Bear::type() const { return "Bear"; }
int Bear::move_distance() const { return 5; }  
int Bear::kill_distance() const { return 10; }  

bool Bear::kills(const NPC& other) const {
    return other.type() == "Knight";
}