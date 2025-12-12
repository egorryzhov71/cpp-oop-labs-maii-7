#ifndef KNIGHT_H
#define KNIGHT_H

#include "NPC.h"

class Knight : public NPC {
public:
    Knight(const std::string& name_, double x_, double y_);
    std::string type() const override;
    int move_distance() const override;    
    int kill_distance() const override;    
    bool kills(const NPC& other) const override;
};

#endif 