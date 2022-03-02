include_directories(include)

add_library(lcms2source
  include/lcms2_plugin.h
  include/lcms2.h
  src/cmsalpha.c
  src/cmscam02.c
  src/cmscgats.c
  src/cmscnvrt.c
  src/cmserr.c
  src/cmsgamma.c
  src/cmsgmt.c
  src/cmshalf.c
  src/cmsintrp.c
  src/cmsio0.c
  src/cmsio1.c
  src/cmslut.c
  src/cmsmd5.c
  src/cmsmtrx.c
  src/cmsnamed.c
  src/cmsopt.c
  src/cmspack.c
  src/cmspcs.c
  src/cmsplugin.c
  src/cmsps2.c
  src/cmssamp.c
  src/cmssm.c
  src/cmstypes.c
  src/cmsvirt.c
  src/cmswtpnt.c
  src/cmsxform.c
  src/lcms2.def
  src/lcms2_internal.h
  )

set_target_properties(lcms2source PROPERTIES VERSION
    ${LCMS2_VERSION})
