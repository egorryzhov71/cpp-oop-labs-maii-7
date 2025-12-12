#ifndef NPCFACTORY_H
#define NPCFACTORY_H

#include "NPC.h"
#include <string>

struct NPCFactory {
    static NPCPtr create(const std::string& type, const std::string& name, double x, double y);
    static NPCPtr from_line(const std::string& line); 
};

#endif 
