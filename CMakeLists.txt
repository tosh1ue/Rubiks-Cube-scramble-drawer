cmake_minimum_required(VERSION 4.1)
project(Rubik_s_Cube_scramble_drawer C)

set(CMAKE_C_STANDARD 99)

include_directories(draw_scramble)

add_executable(Rubik_s_Cube_scramble_drawer
        draw_scramble/draw_scramble.c
        draw_scramble/draw_scramble.h
        example.c)
