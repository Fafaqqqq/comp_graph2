cmake_minimum_required(VERSION 3.21)

set(target lab1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${target})

file(GLOB_RECURSE sources 
    src/*.h
    src/*.cpp
)

set(CMAKE_CXX_STANDARD 14)

add_executable(${target} ${sources})

set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_link_libraries(${target} sfml-graphics sfml-window sfml-system)
