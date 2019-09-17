#!/bin/sh
find ./lib ./generators -iname "*.h" -o -iname "*.cpp" | xargs clang-format -verbose
