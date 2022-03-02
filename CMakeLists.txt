cmake_minimum_required(VERSION 3.16)

project (libicc)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

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

# add the icc library
# To Do: libdicomicc should be built as an External cmake project and
# libicc should be a SuperBuild.
add_subdirectory(libdicomicc)

# copy test dataset
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures/test.dcm
  ${CMAKE_BINARY_DIR}/bin/test.dcm
  COPYONLY)

install(
  FILES ${CMAKE_BINARY_DIR}/bin/test.dcm
  DESTINATION bin
  COMPONENT Runtime)

# js wrapper
if(EMSCRIPTEN)
  add_subdirectory(src)
endif()
