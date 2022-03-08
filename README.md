# libdicomicc

C library for applying [ICC profiles](https://www.color.org/icc_specs2.xalter) to color images.

## C API

The library is a thin wrapper around the established [Little-CMS](https://github.com/mm2/Little-CMS) library and provides a high-level C API geared towards the use within the context of the [Digital Imaging and Communications in Medicine (DICOM)](https://www.dicomstandard.org/) standard.

## JavaScript API

The repository also provides WebAssembly bindings for the C library, which can be build using [Emscripten](https://emscripten.org/), and an object-oriented JavaScript API, which is implemeted in C++ and bound using [Embind](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html).
