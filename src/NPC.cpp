#include "/workspaces/C++/LABA7/include/NPC.h"
#include <cmath>
#include <sstream>
#include <random>
#include <iostream>
#include <chrono>

NPC::NPC(const std::string& name_, double x_, double y_) 
    : name(name_), x(x_), y(y_), alive(true) {}

NPC::~NPC() = default;

double NPC::distance_to(const NPC& other) const {
    std::shared_lock lock(mutex);
    std::shared_lock other_lock(other.mutex);
    
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx*dx + dy*dy);
}

std::string NPC::serialize() const {
    std::shared_lock lock(mutex);
    std::ostringstream oss;
    oss << type() << ";" << name << ";" << x << ";" << y;
    return oss.str();
}

int NPC::roll_dice() {
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> dist(1, 6);
    return dist(rng);
}

void NPC::move_randomly(int map_width, int map_height) {
    if (!alive.load()) return;
    
    std::unique_lock lock(mutex);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> angle_dist(0, 2 * M_PI);
    
    double angle = angle_dist(gen);
    double move_dist = move_distance();
    
    double new_x = x + move_dist * std::cos(angle);
    double new_y = y + move_dist * std::sin(angle);
    
    new_x = std::max(0.0, std::min(static_cast<double>(map_width), new_x));
    new_y = std::max(0.0, std::min(static_cast<double>(map_height), new_y));
    
    x = new_x;
    y = new_y;
}

bool NPC::fight(NPC* attacker) {
    if (!alive.load() || !attacker || !attacker->alive.load()) {
        return false;
    }

    int attack_power = attacker->roll_dice();
    int defense_power = roll_dice();
    
    if (attack_power > defense_power) {
        alive.store(false);
        return true;  
    }
    return false;
}

void NPC::print_info() const {
    std::shared_lock lock(mutex);
    std::cout << name << " (" << type() << ") at (" << x << ", " << y 
              << ") " << (alive.load() ? "ALIVE" : "DEAD") << std::endl;
}