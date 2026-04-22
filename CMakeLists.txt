cmake_minimum_required(VERSION 3.10)
project(rubiks-cube-scramble-engine C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_PROJECT_NAME rubiks_cube_engine)

add_executable(${CMAKE_PROJECT_NAME}
        scramble_generator/scramble_generator.c
        move_applier/move_applier.c
        example.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        scramble_generator
        move_applier
)