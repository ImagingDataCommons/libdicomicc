# Building libicc-js

libicc-js utilizes CMake and EMSCRIPTEN for building.  The dependencies used
for this build are as follows:

* CMake 3.16 or better
* Emscripten SDK 1.39.4 or better
* Modern browser (for running test/browser)

**earlier versions may work but have not been tested**

## Building C example (requires EMSCRIPTEN)

```bash
sh build-native.sh
```

After building, you can run the C based test by executing

```bash
example test.dcm
```

in the folder build/buin

## Building WASM (requires EMSCRIPTEN)

```bash
yarn build
```

After building, you can run the browser based test by running an http server
at the root of this project and opening the file test/browser/index.html.

## Build Output

The build script generates two files in the /dist folder:
* libicc.wasm - WebAssembly build of icc library (wrapper to Little-CMS)
* libicc-js - JavaScript "glue" genereated by EMBIND

## Publishing to NPM

TODO - Add steps to publish 
