cmake_minimum_required(VERSION 4.1)
project(tatlin_tape_core)


add_executable(tatlin_tape_core)
target_sources(tatlin_tape_core
        PUBLIC
        main.cpp
        src/configuration.cpp
        src/exceptions.cpp
        src/tempfile.cpp
        src/tape_processer.cpp
        #############
        include/tape_processer.hpp
        include/configuration.hpp
        include/exceptions.hpp
        include/tempfile.hpp

        PRIVATE
        ../libs/json.hpp # nlohamnn_json for config files
)
