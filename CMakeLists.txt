cmake_minimum_required(VERSION 3.1)

option(BUILD_GENERATOR_GNOME "Build the thumbnail generator for Gnome" ON)
option(BUILD_GENERATOR_KDE "Build the thumbnail generator for KDE" ON)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

project(stl2thumbnail)

add_subdirectory(lib)

if(BUILD_GENERATOR_KDE)
    add_subdirectory(generators/kde)
endif()

if(BUILD_GENERATOR_GNOME)
    add_subdirectory(generators/gnome)
endif()
