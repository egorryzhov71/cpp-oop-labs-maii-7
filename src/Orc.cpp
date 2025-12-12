#include "/workspaces/C++/LABA7/include/Orc.h"

Orc::Orc(const std::string& name_, double x_, double y_) 
    : NPC(name_, x_, y_) {}

std::string Orc::type() const { return "Orc"; }
int Orc::move_distance() const { return 20; }    
int Orc::kill_distance() const { return 10; }    

bool Orc::kills(const NPC& other) const {
    return other.type() == "Bear";
}