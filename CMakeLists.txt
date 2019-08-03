cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)

project(stl2thumbnail)

add_subdirectory(lib)
add_subdirectory(generators/kde)
add_subdirectory(generators/gnome)
