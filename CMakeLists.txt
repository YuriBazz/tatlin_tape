cmake_minimum_required(VERSION 4.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wextra-semi -O3")

project(tatlin_tape)
set(CONFIGS_DIR "${PROJECT_SOURCE_DIR}/samples/configs")
set(TAPES_DIR "${PROJECT_SOURCE_DIR}/samples/tapes")

add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(generator)
add_subdirectory(checker)
