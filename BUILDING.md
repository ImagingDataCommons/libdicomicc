# Building

[CMake](https://cmake.org/) is used to create standard build files.

## Building the native library

### Build dependencies

* CMake 3.16

Earlier versions may work but have not been tested**.

### Build procedure

```none
mkdir -p build
cd build
cmake ..
make
make install
```

### Examples

An example is provided for using libdicomicc with [libdicom](https://github.com/hackermd/libdicom):

```none
cd examples/dicom
mkdir -p build
cd build
cmake ..
make
./example
```

## Building the WASM bindings

### Build dependencies

[Emscripten](https://emscripten.org/) is used to generate WASM bindings and creates the JavaScript API.

The build dependencies used for building the WASM binding are as follows:

* CMake 3.16
* Emscripten SDK 1.39.4

Earlier versions may work but have not been tested.

### Build procedures

```none
yarn run build
```

### Examples

After building, you can run the browser based test by running an web server at
the ``wasm/examples`` directory:

```none
cd wasm/examples
python -m SimpleHTTPServer 3000
```

and then pointing the browser to ``http://localhost:3000/browser``.
