cmake_minimum_required(VERSION 3.12)
project(LR1)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)
include_directories(src/)

add_executable(LR1 src/main.cpp src/LRParser.cpp tests/tests.cpp src/LRParser.h)

target_link_libraries(LR1 gtest gtest_main)