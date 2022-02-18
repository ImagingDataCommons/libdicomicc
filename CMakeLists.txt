cmake_minimum_required (VERSION 3.22.2)

project (libdicomicc)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# To Do: libdicom should be built as an External cmake project and
# this project should be a SuperBuild. However to do this,
# we need to transfom libdicom in a CMake project.
set(LIBDICOM_VERSION_MAJOR 0)
set(LIBDICOM_VERSION_MINOR 1)
set(LIBDICOM_VERSION_PATCH 0)
set(LIBDICOM_VERSION "${LIBDICOM_VERSION_MAJOR}.${LIBDICOM_VERSION_MINOR}.${LIBDICOM_VERSION_PATCH}")

# To Do: lcms2 should be built as an External cmake project and
# this project should be a SuperBuild. However to do this,
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

# Compiler options
# "-Wl,--disable-new-dtags" is needed for install and packing (i.e., force to update rpath for not direct dependencies).
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -ftree-vectorize -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ftree-vectorize -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O2 -ftree-vectorize -fPIC -flto")

# Install and RPath settings
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/libicc-install")
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#================================
# Building
#================================
set(BUILD_SHARED_LIBS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(libdicom)
include_directories(libdicom/include)

add_subdirectory(lcms2)
include_directories(lcms2/include)

include_directories(src)
add_library(libdicomicc src/icc.h src/icc.c)
set_target_properties(libdicomicc PROPERTIES VERSION
    ${PROJECT_VERSION})
target_link_libraries(libdicomicc lcms2source m)

add_executable(example examples/dicom/example.c)
target_link_libraries(example libdicomicc dicom)

#================================
# Install
#================================
include(InstallRequiredSystemLibraries)

install(TARGETS libdicomicc DESTINATION lib COMPONENT Runtime)
install(TARGETS dicom DESTINATION lib COMPONENT Runtime)
install(TARGETS example DESTINATION bin COMPONENT Runtime)

#================================
# Packaging
#================================

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libdicomicc")
set(CPACK_PACKAGE_VENDOR "libdicomicc")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_GENERATOR "TGZ")
include(CPack)
