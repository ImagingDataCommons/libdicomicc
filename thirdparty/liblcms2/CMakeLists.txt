include(ExternalProject)
ExternalProject_Add(lcms2download
  PREFIX            lcms2-build
  URL               https://github.com/mm2/Little-CMS/releases/download/lcms2.13.1/lcms2-2.13.1.zip
  URL_HASH          SHA256=4ea5cba9772182e641bf7acc2a4ee65f73b3718d47bcc4e71381e1b7b01a49d7
  BUILD_IN_SOURCE   1
  CONFIGURE_COMMAND ./configure -enable-static --prefix=${CMAKE_BINARY_DIR}/thirdparty/lcms2-install
  BUILD_COMMAND     make
  INSTALL_COMMAND   make install
  TEST_COMMAND      ""
  UPDATE_COMMAND    ""
  )

set(LCMS2_LIBRARY "${CMAKE_BINARY_DIR}/thirdparty/lcms2-install/lib/liblcms2.a" CACHE INTERNAL "" FORCE)
set(LCMS2_INCLUDE_DIR "${CMAKE_BINARY_DIR}/thirdparty/lcms2-install/include" CACHE INTERNAL "" FORCE)
