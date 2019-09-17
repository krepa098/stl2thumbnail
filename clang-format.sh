#!/bin/sh
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
find $SCRIPTPATH/lib $SCRIPTPATH/generators -iname "*.h" -o -iname "*.cpp" | xargs clang-format -verbose -i
