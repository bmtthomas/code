cmake_minimum_required(VERSION 3.26)
project(plateau_de_jeu C)

set(CMAKE_C_STANDARD 99)

add_executable(plateau_de_jeu main.c
        header.h
        fcts.c)
