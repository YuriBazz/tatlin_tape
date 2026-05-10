cmake_minimum_required(VERSION 4.1)
project(tatlin_tape_tests)

add_executable(tatlin_tape_tests)
target_sources(tatlin_tape_tests
        PUBLIC
        tests.cpp
        PRIVATE
        ../core/src/configuration.cpp
        ../core/src/exceptions.cpp
        ../core/src/tempfile.cpp
        ../core/src/tape_processer.cpp
        #############
        ../core/include/tape_processer.hpp
        ../core/include/configuration.hpp
        ../core/include/exceptions.hpp
        ../core/include/tempfile.hpp
        ../libs/simple_test.h
)

target_compile_definitions(tatlin_tape_tests
        PRIVATE
        CONFIGS_DIR="${CONFIGS_DIR}"
        TAPES_DIR="${TAPES_DIR}"

)