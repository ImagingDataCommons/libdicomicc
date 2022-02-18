# libicc
C library for using ICC profiles to correct color images

## Installation

```
git clone https://github.com/hackermd/libicc 
mkdir libicc-Build
cd libicc-Build
cmake ../libicc -DCMAKE_BUILD_TYPE:STRING=DEBUG
make
```

## Packing

```
cd libicc-Build
cpack --config ./CPackConfig.cmake
```
