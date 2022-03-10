#================================
# Building
#================================
if(EMSCRIPTEN)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

find_library(LCMS2_LIBRARY lcms2 PATH_SUFFIXES lcms2)
find_path(LCMS2_INCLUDE_DIR lcms2.h PATH_SUFFIXES lcms2)
if(NOT LCMS2_LIBRARY)
message(FATAL_ERROR "lcm2 library is required but was not found")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(DICOMICC_LIBRARY "dicomicc" CACHE INTERNAL "" FORCE)
set(DICOMICC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "" FORCE)

add_library(${DICOMICC_LIBRARY} dicomicc.h dicomicc.c)
set_target_properties(${DICOMICC_LIBRARY} PROPERTIES VERSION ${DICOMICC_VERSION})
target_link_libraries(${DICOMICC_LIBRARY} ${LCMS2_LIBRARY} m)
target_include_directories(${DICOMICC_LIBRARY} PRIVATE ${LCMS2_INCLUDE_DIR})

#================================
# Install
#================================
include(InstallRequiredSystemLibraries)

install(TARGETS ${DICOMICC_LIBRARY} DESTINATION lib COMPONENT Runtime)
install(FILES "${PROJECT_SOURCE_DIR}/src/dicomicc.h" DESTINATION include)

#================================
# Packaging
#================================

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DICOMICC_LIBRARY})
set(CPACK_PACKAGE_VENDOR ${DICOMICC_LIBRARY})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR ${DICOMICC_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${DICOMICC_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${DICOMICC_VERSION_PATCH})
set(CPACK_GENERATOR "TGZ")
include(CPack)
