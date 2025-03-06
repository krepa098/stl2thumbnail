cmake_minimum_required(VERSION 3.16)

project(stl2thumbnail CXX)

option(BUILD_GENERATOR_GNOME "Build the thumbnail generator for Gnome" ON)
option(BUILD_GENERATOR_KDE "Build the thumbnail generator for KDE" ON)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib)

if(BUILD_GENERATOR_KDE)
    add_subdirectory(generators/kde)
endif()

if(BUILD_GENERATOR_GNOME)
    add_subdirectory(generators/gnome)
endif()
