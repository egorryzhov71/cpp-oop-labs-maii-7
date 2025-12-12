cmake_minimum_required(VERSION 3.10)
project(dungeon_threaded)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES
    src/NPC.cpp
    src/Orc.cpp
    src/Knight.cpp
    src/Bear.cpp
    src/Observer.cpp
    src/NPCFactory.cpp
    src/ThreadedGame.cpp
)

add_executable(dungeon_threaded
    main.cpp
    ${SOURCES}
)

target_include_directories(dungeon_threaded PRIVATE include)
target_link_libraries(dungeon_threaded Threads::Threads)

find_package(GTest QUIET)
if(GTest_FOUND)
    add_executable(dungeon_tests
        test/tests.cpp
        ${SOURCES}
    )
    target_include_directories(dungeon_tests PRIVATE include)
    target_link_libraries(dungeon_tests GTest::gtest GTest::gtest_main Threads::Threads)
endif()