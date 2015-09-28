#!/bin/bash

mkdir -p build && cd build
# Trying to fix bypass issue 1...
export LD_LIBRARY_PATH="${LD_LIBRAIRY_PATH}:/usr/local/lib/gcc/x86_64-pc-linux-gnu/lib64/"
cmake .. -DCMAKE_CXX_COMPILER=$(which g++-concepts)
ctest --verbose
