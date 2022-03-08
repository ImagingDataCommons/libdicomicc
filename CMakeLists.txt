cmake_minimum_required(VERSION 3.16)

project (dicomicc)

set(DICOMICC_VERSION_MAJOR 0)
set(DICOMICC_VERSION_MINOR 1)
set(DICOMICC_VERSION_PATCH 0)
set(DICOMICC_VERSION "${DICOMICC_VERSION_MAJOR}.${DICOMICC_VERSION_MINOR}.${DICOMICC_VERSION_PATCH}")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h")

# Compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ftree-vectorize -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -ftree-vectorize -fPIC -flto")

# set the build type if not specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

if(EMSCRIPTEN)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

add_subdirectory(thirdparty)

add_subdirectory(src)

# js wrapper
if(EMSCRIPTEN)
  add_subdirectory(wasm)
endif()
