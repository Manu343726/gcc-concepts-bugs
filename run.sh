#!/bin/bash

mkdir -p build && cd build
cmake .. -DCMAKE_CXX_COMPILER=$(which g++-concepts)
ctest --verbose