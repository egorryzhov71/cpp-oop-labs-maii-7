#include <gtest/gtest.h>
#include "/workspaces/C++/LABA7/include/NPC.h"
#include "/workspaces/C++/LABA7/include/Orc.h"
#include "/workspaces/C++/LABA7/include/Knight.h"
#include "/workspaces/C++/LABA7/include/Bear.h"
#include "/workspaces/C++/LABA7/include/ThreadedGame.h"
#include <memory>
#include <thread>
#include <chrono>

TEST(ThreadedNPCTest, MoveDistancesFromTable) {
    auto orc = std::make_shared<Orc>("Orc1", 0, 0);
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto bear = std::make_shared<Bear>("Bear1", 0, 0);
    
    EXPECT_EQ(orc->move_distance(), 20);     
    EXPECT_EQ(knight->move_distance(), 30); 
    EXPECT_EQ(bear->move_distance(), 5);   
}

TEST(ThreadedNPCTest, KillDistancesFromTable) {
    auto orc = std::make_shared<Orc>("Orc1", 0, 0);
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto bear = std::make_shared<Bear>("Bear1", 0, 0);
    
    EXPECT_EQ(orc->kill_distance(), 10);      
    EXPECT_EQ(knight->kill_distance(), 10);
    EXPECT_EQ(bear->kill_distance(), 10);
}

TEST(ThreadedNPCTest, KillLogicRemains) {
    auto orc = std::make_shared<Orc>("Orc1", 0, 0);
    auto knight = std::make_shared<Knight>("Knight1", 0, 0);
    auto bear = std::make_shared<Bear>("Bear1", 0, 0);
    
    EXPECT_TRUE(knight->kills(*orc));
    EXPECT_FALSE(knight->kills(*bear));

    EXPECT_TRUE(orc->kills(*bear));
    EXPECT_FALSE(orc->kills(*knight));
    
    EXPECT_TRUE(bear->kills(*knight));
    EXPECT_FALSE(bear->kills(*orc));
}

TEST(ThreadedNPCTest, DiceRollInRange) {
    auto npc = std::make_shared<Orc>("Test", 0, 0);
    
    for (int i = 0; i < 100; i++) {
        int roll = npc->roll_dice();
        EXPECT_GE(roll, 1);
        EXPECT_LE(roll, 6);
    }
}

TEST(ThreadedNPCTest, FightMechanics) {
    auto attacker = std::make_shared<Knight>("Attacker", 0, 0);
    auto defender = std::make_shared<Orc>("Defender", 0, 0);
    
    attacker->alive = true;
    defender->alive = true;
    
    EXPECT_NO_THROW(defender->fight(attacker.get()));
    
}

TEST(ThreadedNPCTest, MovementWithinBounds) {
    auto npc = std::make_shared<Orc>("Mover", 50, 50);
    npc->alive = true;
    
    for (int i = 0; i < 100; i++) {
        npc->move_randomly(100, 100);
        
        EXPECT_GE(npc->x, 0);
        EXPECT_LE(npc->x, 100);
        EXPECT_GE(npc->y, 0);
        EXPECT_LE(npc->y, 100);
    }
}

TEST(ThreadedNPCTest, DeadNPCsDontMove) {
    auto npc = std::make_shared<Bear>("DeadBear", 10, 10);
    npc->alive = false;
    
    double initial_x = npc->x;
    double initial_y = npc->y;
    
    npc->move_randomly(100, 100);
    
    EXPECT_DOUBLE_EQ(npc->x, initial_x);
    EXPECT_DOUBLE_EQ(npc->y, initial_y);
}

TEST(ThreadedGameTest, GameInitialization) {
    ThreadedGame game;
    
    EXPECT_NO_THROW(game.initialize());
}


TEST(ThreadedNPCTest, MultipleMovementsStayInBounds) {
    std::vector<std::shared_ptr<NPC>> npcs;
    
    npcs.push_back(std::make_shared<Orc>("Orc1", 0, 0));
    npcs.push_back(std::make_shared<Knight>("Knight1", 100, 100));
    npcs.push_back(std::make_shared<Bear>("Bear1", 50, 50));
    
    for (auto& npc : npcs) {
        npc->alive = true;
        
        for (int i = 0; i < 1000; i++) {
            npc->move_randomly(100, 100);
            
            ASSERT_GE(npc->x, 0) << "NPC " << npc->name << " вышел за левую границу";
            ASSERT_LE(npc->x, 100) << "NPC " << npc->name << " вышел за правую границу";
            ASSERT_GE(npc->y, 0) << "NPC " << npc->name << " вышел за нижнюю границу";
            ASSERT_LE(npc->y, 100) << "NPC " << npc->name << " вышел за верхнюю границу";
        }
    }
}

TEST(ThreadedNPCTest, DistanceCalculation) {
    auto npc1 = std::make_shared<Orc>("Npc1", 0, 0);
    auto npc2 = std::make_shared<Knight>("Npc2", 3, 4);
    

    EXPECT_DOUBLE_EQ(npc1->distance_to(*npc2), 5.0);
    

    auto npc3 = std::make_shared<Bear>("Npc3", 10, 20);
    auto npc4 = std::make_shared<Bear>("Npc4", 10, 20);
    EXPECT_DOUBLE_EQ(npc3->distance_to(*npc4), 0.0);
}


TEST(ThreadedNPCTest, AtomicAliveFlag) {
    auto npc = std::make_shared<Orc>("AtomicTest", 0, 0);
    

    EXPECT_TRUE(npc->alive);
    

    npc->alive = false;
    EXPECT_FALSE(npc->alive);
    

    npc->alive = true;
    EXPECT_TRUE(npc->alive);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}