#pragma once
#include "NPC.h"
#include "Knight.h"
#include "Orc.h"

class Bear : public NPC {
public:
    Bear(int x, int y, const std::string& name);
    Bear(std::istream &is);
    
    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Knight> other) override;
    bool fight(std::shared_ptr<Orc> other) override;
    bool fight(std::shared_ptr<Bear> other) override;
    
    void print() override;
    void save(std::ostream &os) override; 

    int get_move_distance() const override { return 5; }
    int get_kill_distance() const override { return 10; }
};