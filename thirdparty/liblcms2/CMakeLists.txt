project(liblcms2)

if(EMSCRIPTEN)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

#================================
# Building
#================================
set(LCMS2_LIBRARY "lcms2" CACHE INTERNAL "" FORCE)
set(LCMS2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" FORCE)


file(GLOB SRCS src/*.c)
file(GLOB HDRS include/*.h)

add_library(${LCMS2_LIBRARY} STATIC ${SRCS} ${HDRS})
target_include_directories(${LCMS2_LIBRARY} PRIVATE "${LCMS2_INCLUDE_DIR}")
set_target_properties(${LCMS2_LIBRARY}
                      PROPERTIES
                      OUTPUT_NAME "${LIBTARGET}"
                      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/lib)

#================================
# Install
#================================
include(InstallRequiredSystemLibraries)

install(TARGETS ${LCMS2_LIBRARY} DESTINATION lib COMPONENT Runtime)
