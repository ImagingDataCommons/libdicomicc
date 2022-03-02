include_directories(include)

add_library(dicom
  include/dicom.h
  include/utarray.h
  include/uthash.h
  src/dicom.c
  src/dicom-data.c
  src/dicom-dict.c
  src/dicom-file.c)

set_target_properties(dicom PROPERTIES VERSION
    ${LIBDICOM_VERSION})
