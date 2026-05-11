cmake_minimum_required(VERSION 4.1)
project(tatlin_tape_tests)

add_executable(tatlin_tape_tests tests.cpp)
target_link_libraries(tatlin_tape_tests PRIVATE tatlin_tape_core_lib)
target_include_directories(tatlin_tape_tests PRIVATE ../libs)

target_compile_definitions(tatlin_tape_tests
        PRIVATE
        CONFIGS_DIR="${CONFIGS_DIR}"
        TAPES_DIR="${TAPES_DIR}"

)