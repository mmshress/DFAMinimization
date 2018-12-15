cmake_minimum_required(VERSION 3.12)
project(DFAMinimization)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(hopcroft Automaton.cpp Partition.cpp Hopcroft.cpp mainHopCroft.cpp)
add_executable(valmari Valmari.cpp)
add_executable(generator AutomataGen.cpp)
