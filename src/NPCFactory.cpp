#include "/workspaces/C++/LABA7/include/NPCFactory.h"
#include "/workspaces/C++/LABA7/include/Orc.h"
#include "/workspaces/C++/LABA7/include/Knight.h"
#include "/workspaces/C++/LABA7/include/Bear.h"
#include <sstream>
#include <vector>

NPCPtr NPCFactory::create(const std::string& type, const std::string& name, double x, double y) {
    if (type == "Orc") return std::make_shared<Orc>(name, x, y);
    if (type == "Knight") return std::make_shared<Knight>(name, x, y);
    if (type == "Bear") return std::make_shared<Bear>(name, x, y);
    throw std::runtime_error("Unknown NPC type: " + type);
}

NPCPtr NPCFactory::from_line(const std::string& line) {
    std::vector<std::string> parts;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ';')) parts.push_back(token);
    if (parts.size() != 4) throw std::runtime_error("Bad line format: " + line);
    double x = std::stod(parts[2]);
    double y = std::stod(parts[3]);
    return create(parts[0], parts[1], x, y);
}
