cmake_minimum_required (VERSION 3.16)

project (libdicomicc)

# To Do: libdicomicc should be built as an External cmake project and
# libicc should be a SuperBuild.
set(LIBDICOMICC_VERSION_MAJOR 0)
set(LIBDICOMICC_VERSION_MINOR 1)
set(LIBDICOMICC_VERSION_PATCH 0)
set(LIBDICOMICC_VERSION "${LIBDICOMICC_VERSION_MAJOR}.${LIBDICOMICC_VERSION_MINOR}.${LIBDICOMICC_VERSION_PATCH}")

# To Do: libdicom should be built as an External cmake project and
# libicc should be a SuperBuild. However to do this,
# we need to transfom libdicom in a CMake project.
set(LIBDICOM_VERSION_MAJOR 0)
set(LIBDICOM_VERSION_MINOR 1)
set(LIBDICOM_VERSION_PATCH 0)
set(LIBDICOM_VERSION "${LIBDICOM_VERSION_MAJOR}.${LIBDICOM_VERSION_MINOR}.${LIBDICOM_VERSION_PATCH}")

# To Do: lcms2 should be built as an External cmake project and
# libicc should be a SuperBuild. However to do this,
# we need to transfom lcms2 in a CMake project.
set(LCMS2_VERSION_MAJOR 0)
set(LCMS2_VERSION_MINOR 1)
set(LCMS2_VERSION_PATCH 0)
set(LCMS2_VERSION "${LCMS2_VERSION_MAJOR}.${LCMS2_VERSION_MINOR}.${LCMS2_VERSION_PATCH}")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h")

# add the binary tree to the search path for include files
# so that we will find ExampleConfig.h
include_directories("${PROJECT_BINARY_DIR}")

#================================
# Building
#================================
if(EMSCRIPTEN)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(libdicom)
include_directories(libdicom/include)

add_subdirectory(lcms2)
include_directories(lcms2/include)

include_directories(src)
add_library(dicomicc src/icc.h src/icc.c)
set_target_properties(dicomicc PROPERTIES VERSION
    ${LIBDICOMICC_VERSION})
target_link_libraries(dicomicc lcms2source m)

if(NOT EMSCRIPTEN)
  add_executable(example examples/dicom/example.c)
  target_link_libraries(example dicomicc dicom)
endif()

#================================
# Install
#================================
include(InstallRequiredSystemLibraries)

install(TARGETS dicomicc DESTINATION lib COMPONENT Runtime)
install(TARGETS dicom DESTINATION lib COMPONENT Runtime)
install(TARGETS lcms2source DESTINATION lib COMPONENT Runtime)

if(NOT EMSCRIPTEN)
  install(TARGETS example DESTINATION bin COMPONENT Runtime)
endif()

#================================
# Packaging
#================================

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libdicomicc")
set(CPACK_PACKAGE_VENDOR "libdicomicc")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR ${LIBDICOMICC_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${LIBDICOMICC_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${LIBDICOMICC_VERSION_PATCH})
set(CPACK_GENERATOR "TGZ")
include(CPack)
