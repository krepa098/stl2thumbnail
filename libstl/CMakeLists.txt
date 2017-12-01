cmake_minimum_required(VERSION 3.0)

project(libstl)
add_library(
    ${PROJECT_NAME}
    "parser.h"
    "parser.cpp"
    "helpers.h"
    )
