#ifndef ORC_H
#define ORC_H

#include "NPC.h"

class Orc : public NPC {
public:
    Orc(const std::string& name_, double x_, double y_);
    std::string type() const override;
    int move_distance() const override;    
    int kill_distance() const override;    
    bool kills(const NPC& other) const override;
};

#endif 