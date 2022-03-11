#!/bin/bash
# Disable exit on non 0
set -euo pipefail

mkdir -p build
mkdir -p dist

# DEBUG CONFIGURE
#(cd build && emcmake cmake -DCMAKE_BUILD_TYPE=Debug ..)

echo "~~~ CONFIGURE ~~~"
(cd build && emcmake cmake ..)
echo "~~~ MAKE ~~~"
(cd build && emmake make)
echo "~~~ COPY ~~~ "
cp ./build/wasm/dicomiccwasm.js ./dist
cp ./build/wasm/dicomiccwasm.wasm ./dist

echo "~~~ BUILD:"
(cd build && dir)
echo "~~~ DIST:"
(cd dist && dir)
