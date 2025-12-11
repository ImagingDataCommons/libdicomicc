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

An C example is provided for using the dicomicc library with the [dicom](https://github.com/hackermd/libdicom):

```none
cd examples/dicom
mkdir -p build
cd build
cmake ..
make
./bin/dicomicc-example ...
```

The examples expects the lcms2, dicom, and dicomicc libraries to be already installed.

## Building the WASM bindings

### Build dependencies

[Emscripten](https://emscripten.org/) is used to generate WASM bindings and creates the JavaScript API.

The build dependencies used for building the WASM binding are as follows:

* CMake 3.16
* Emscripten SDK 4.19.0

Earlier versions may work but have not been tested.

### Build procedures

```none
yarn run build
```

After the build procedure, the generated JavaScript and WebAssembly files will be located in ``dist``.

### Examples

After building the WebAssembly bindings and JavaScript API, the browser based example can be run by starting a web server in the ``root`` directory:

```none
python -m SimpleHTTPServer  # Python 2
```

and then pointing the browser to ``http://localhost:8000/wasm/examples/browser/``.
