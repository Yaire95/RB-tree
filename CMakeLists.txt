cmake_minimum_required(VERSION 3.16)
project(ex3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex3 RBTree.c Structs.h Structs.c tests.c RButilities.c)