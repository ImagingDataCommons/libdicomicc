#!/bin/sh
# Disable exit on non 0
set +e
mkdir -p build
mkdir -p dist

# DEBUG CONFIGURE
#(cd build && emcmake cmake -DCMAKE_BUILD_TYPE=Debug ..)

echo "~~~ CONFIGURE ~~~"
(cd build && emcmake cmake ..)
echo "~~~ MAKE ~~~"
(cd build && emmake make -j 8)
echo "~~~ COPY ~~~ "
cp ./build/src/libiccwasm.js ./dist
cp ./build/src/libiccwasm.wasm ./dist
cp ./build/src/libiccjs.js ./dist
cp ./build/src/libiccjs.js.mem ./dist

echo "~~~ BUILD:"
(cd build && dir)
echo "~~~ DIST:"
(cd dist && dir)
