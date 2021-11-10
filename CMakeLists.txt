cmake_minimum_required(VERSION 3.20)
project(AVL_tree C)

set(CMAKE_C_STANDARD 99)

add_executable(AVL_tree main.c AVLTree.h AVLTree.c)
