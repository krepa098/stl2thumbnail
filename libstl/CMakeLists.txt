cmake_minimum_required(VERSION 3.1)

project(stl)

add_library(
    ${PROJECT_NAME}
    "parser.h"
    "parser.cpp"
    "helpers.h"
)
