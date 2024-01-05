#!/bin/bash

set -xe

mkdir -p build

FLAGS="-std=c++17 -g -Wall -Werror -Wextra -pedantic -pedantic-errors"
OUTPUT="-o build/main"
INPUT="main.cpp parser.cpp"

g++ $OUTPUT $INPUT $FLAGS

if [ $# -eq 1 ] && [ $1 == "run" ]; then
    ./build/main
fi
