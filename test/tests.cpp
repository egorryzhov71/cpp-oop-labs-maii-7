#include <gtest/gtest.h>
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/NPC.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Orc.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Bear.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/Knight.h"
#include "/workspaces/c++/cpp-oop-labs-maii-7/include/NPCFactory.h"

#include <sstream>
#include <fstream>

TEST(NPCTest, FactoryCreatesNPCs) {
    auto knight = NPCFactory::factory(NPC::Type::Knight, 10, 20, "Arthur");
    auto orc = NPCFactory::factory(NPC::Type::Orc, 30, 40, "Grom");
    auto bear = NPCFactory::factory(NPC::Type::Bear, 50, 60, "Misha");
    
    EXPECT_NE(knight, nullptr) << "Failed to create Knight";
    EXPECT_NE(orc, nullptr) << "Failed to create Orc";
    EXPECT_NE(bear, nullptr) << "Failed to create Bear";
    
    EXPECT_TRUE(knight->is_alive()) << "Knight should be alive after creation";
    EXPECT_TRUE(orc->is_alive()) << "Orc should be alive after creation";
    EXPECT_TRUE(bear->is_alive()) << "Bear should be alive after creation";
}

TEST(NPCTest, UniqueNames) {
    auto factory = NPCFactory();
    
    auto npc1 = factory.create(NPC::Type::Knight, 10, 20, "UniqueName");
    EXPECT_NE(npc1, nullptr) << "Should create NPC with name 'UniqueName'";
    
    auto npc2 = factory.create(NPC::Type::Orc, 30, 40, "UniqueName");
    EXPECT_EQ(npc2, nullptr) << "Should NOT create NPC with duplicate name";
}
TEST(NPCTest, DistanceCheck) {
    auto npc1 = std::make_shared<Knight>(0, 0, "K1");
    auto npc2 = std::make_shared<Knight>(3, 4, "K2"); 
    auto npc3 = std::make_shared<Knight>(30, 40, "K3"); 
    
    EXPECT_TRUE(npc1->is_close(npc2, 10)) << "NPCs should be close (distance 5 < 10)";
    EXPECT_FALSE(npc1->is_close(npc3, 10)) << "NPCs should NOT be close (distance 50 > 10)";
    EXPECT_TRUE(npc1->is_close(npc3, 60)) << "NPCs should be close (distance 50 < 60)";
}


TEST(NPCTest, ObserverSingleton) {
    auto observer1 = TextObserver::get();
    auto observer2 = TextObserver::get();
    
    EXPECT_EQ(observer1.get(), observer2.get()) 
        << "TextObserver::get() should return same instance";
}