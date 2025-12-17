cmake_minimum_required(VERSION 3.10)
project(balagur_dungeon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/Bear.cpp
    src/Knight.cpp
    src/NPC.cpp
    src/Orc.cpp
)

set(TEST_SOURCES
    test/tests.cpp
)


add_executable(dungeon_editor
    main.cpp
    ${SOURCES}
)

add_executable(dungeon_tests
    ${TEST_SOURCES}
    ${SOURCES}
)

target_link_libraries(dungeon_tests GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(dungeon_tests)