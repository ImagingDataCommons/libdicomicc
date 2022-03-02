#!/bin/sh
mkdir -p build
#(cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..)
(cd build && cmake ..)
(cd build && make -j 8)
