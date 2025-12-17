#pragma once
#include "NPC.h"
#include "Orc.h"
#include "Bear.h"

class Knight : public NPC {
public:
    Knight(int x, int y, const std::string& name);
    Knight(std::istream &is);
    
    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Knight> other) override;
    bool fight(std::shared_ptr<Orc> other) override;
    bool fight(std::shared_ptr<Bear> other) override;
    
    void print() override;
    void save(std::ostream &os) override;

    int get_move_distance() const override { return 30; }
    int get_kill_distance() const override { return 10; }
};