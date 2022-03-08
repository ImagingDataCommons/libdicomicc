#!/bin/sh
# Disable exit on non 0
set -uE

mkdir -p build
mkdir -p dist

# DEBUG CONFIGURE
#(cd build && emcmake cmake -DCMAKE_BUILD_TYPE=Debug ..)

echo "~~~ CONFIGURE ~~~"
(cd build && emcmake cmake ..)
echo "~~~ MAKE ~~~"
(cd build && emmake make -j 8)
echo "~~~ COPY ~~~ "
cp ./build/wasm/dicomicc.js ./dist
cp ./build/wasm/dicomicc.wasm ./dist

echo "~~~ BUILD:"
(cd build && dir)
echo "~~~ DIST:"
(cd dist && dir)
