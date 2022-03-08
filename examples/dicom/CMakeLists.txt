#================================
# Building
# NOTE: DICOM library is not included in the thirdparty build.
#       https://github.com/hackermd/libdicom
#================================

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${LCMS2_INCLUDE_DIR} ${DCMICC_INCLUDE_DIR} ${DICOM_INCLUDE_DIR})

add_executable(test example.c)
target_link_libraries(test deepLook ${LCMS2_LIBRARIES} ${DCMICC_LIBRARIES} ${DICOM_LIBRARIES})


#================================
# Install
#================================
include(InstallRequiredSystemLibraries)

install(TARGETS test DESTINATION bin COMPONENT Runtime)
