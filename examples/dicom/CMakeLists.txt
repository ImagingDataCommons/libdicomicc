#================================
# Building
# NOTE: DICOM library is not included in the thirdparty build.
#       https://github.com/hackermd/libdicom
#================================
cmake_minimum_required(VERSION 3.16)

project (libdicomicc-example)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_library(LCMS2_LIBRARY lcms2 PATH_SUFFIXES lcms2)
find_path(LCMS2_INCLUDE_DIR lcms2.h PATH_SUFFIXES lcms2)
if(NOT LCMS2_LIBRARY)
  message(FATAL_ERROR "lcm2 library is required but was not found")
endif()

find_library(DICOM_LIBRARY dicom PATH_SUFFIXES dicom)
find_path(DICOM_INCLUDE_DIR dicom.h PATH_SUFFIXES dicom)
if(NOT DICOM_LIBRARY)
  message(FATAL_ERROR "dicom library is required but was not found")
endif()

find_library(DICOMICC_LIBRARY dicomicc PATH_SUFFIXES dicomicc)
find_path(DICOMICC_INCLUDE_DIR dicomicc.h PATH_SUFFIXES dicomicc)
if(NOT DICOMICC_LIBRARY)
  message(FATAL_ERROR "dicomicc library is required but was not found")
endif()


add_executable(dicomicc-example example.c)
target_include_directories(dicomicc-example PUBLIC
                           ${LCMS2_INCLUDE_DIR}
                           ${DICOM_INCLUDE_DIR}
                           ${DICOMICC_INCLUDE_DIR})
target_link_libraries(dicomicc-example
                       ${LCMS2_LIBRARY}
                       ${DICOM_LIBRARY}
                       ${DICOMICC_LIBRARY})

#================================
# Install
#================================
include(InstallRequiredSystemLibraries)

install(TARGETS dicomicc-example DESTINATION bin COMPONENT Runtime)
