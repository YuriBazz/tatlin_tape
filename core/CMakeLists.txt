cmake_minimum_required(VERSION 4.1)
project(tatlin_tape_core)

add_library(tatlin_tape_core_lib STATIC
        src/configuration.cpp
        src/exceptions.cpp
        src/tempfile.cpp
        src/tape_processer.cpp
        src/sorter.cpp
)

target_include_directories(tatlin_tape_core_lib PUBLIC include ../libs)

add_executable(tatlin_tape_core main.cpp)
target_link_libraries(tatlin_tape_core PRIVATE tatlin_tape_core_lib)
