#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lcms2.h>

#include "icc.h"
#include "config.h"

struct _DmcIccTransform {
    cmsHTRANSFORM handle;
};

const char *dcm_icc_get_version(void) {
    return PROJECT_VER;
}

DmcIccTransform *dcm_icc_transform_create(const char *icc_profile,
                                          uint32_t icc_profile_size,
                                          uint8_t planar_configuration) {
    cmsUInt32Number type;

    // Input ICC profile: obtained from DICOM data set
    const cmsHPROFILE in_handle  = cmsOpenProfileFromMem(icc_profile,
                                                         icc_profile_size);

    if (in_handle == NULL) {
        return NULL;
    }

    // Output ICC profile: fixed to sRGB
    const cmsHPROFILE out_handle = cmsCreate_sRGBProfile();

    if (out_handle == NULL) {
        cmsCloseProfile(in_handle);
        return NULL;
    }

    if (planar_configuration == 1) {
        type = TYPE_RGB_8_PLANAR;
    } else {
        type = TYPE_RGB_8;
    }


    const cmsHTRANSFORM transform_handle = cmsCreateTransform(in_handle,
                                                              type,
                                                              out_handle,
                                                              type,
                                                              INTENT_PERCEPTUAL,
                                                              0);

    cmsCloseProfile(in_handle);
    cmsCloseProfile(out_handle);

    if (transform_handle == NULL) {
        return NULL;
    }

    DmcIccTransform *icc_transform = calloc(1, sizeof(DmcIccTransform));
    if (icc_transform == NULL) {
        cmsDeleteTransform(transform_handle);
        return NULL;
    }

    icc_transform->handle = transform_handle;

    return icc_transform;
}

void dcm_icc_transform_apply(const DmcIccTransform *icc_transform,
                             const char *frame,
                             uint16_t columns,
                             uint16_t rows,
                             char *corrected_frame) {
    cmsDoTransform(icc_transform->handle,
                   frame,
                   corrected_frame,
                   columns * rows);
}

void dcm_icc_transform_destroy(DmcIccTransform *icc_transform) {
    if (icc_transform) {
        if (icc_transform->handle) {
            cmsDeleteTransform(icc_transform->handle);
            icc_transform->handle = NULL;
        }
        free(icc_transform);
        icc_transform = NULL;
    }
}
