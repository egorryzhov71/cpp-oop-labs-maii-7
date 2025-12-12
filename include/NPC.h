#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <shared_mutex>

class NPC {
public:
    std::string name;
    double x, y;
    std::atomic<bool> alive;
    mutable std::shared_mutex mutex;
    
    NPC(const std::string& name_, double x_, double y_);
    virtual ~NPC();
    
    virtual std::string type() const = 0;
    virtual int move_distance() const = 0;    
    virtual int kill_distance() const = 0;    
    
    double distance_to(const NPC& other) const;
    virtual bool kills(const NPC& other) const = 0;
    
    virtual std::string serialize() const;
    
    void move_randomly(int map_width = 100, int map_height = 100);
    bool fight(NPC* attacker);  
    void print_info() const;
    
    static int roll_dice();
};

using NPCPtr = std::shared_ptr<NPC>;

#endif 