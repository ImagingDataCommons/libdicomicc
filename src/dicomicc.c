#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lcms2.h>

#include "dicomicc.h"
#include "config.h"

struct _DmcIccTransform {
    cmsHTRANSFORM handle;
    uint32_t number_of_pixels;
};

const char *dcm_icc_get_version(void) {
    return DCMICC_VERSION;
}

DmcIccTransform *dcm_icc_transform_create_for_output(const char *icc_profile,
                                                     uint32_t icc_profile_size,
                                                     uint8_t planar_configuration,
                                                     uint16_t columns,
                                                     uint16_t rows,
                                                     DcmIccOutputType output_type) {
    cmsUInt32Number type;

    // Input ICC profile: obtained from DICOM data set
    const cmsHPROFILE in_handle = cmsOpenProfileFromMem(icc_profile,
                                                        icc_profile_size);
    cmsHPROFILE out_handle = NULL;

    if (in_handle == NULL) {
        return NULL;
    }

    // sRGB output ICC profile (use build-in littleCMS functionality)
    if (output_type == DCM_ICC_OUTPUT_SRGB) {
        // Output ICC profile: fixed to sRGB
        out_handle = cmsCreate_sRGBProfile();

    // Display-P3 output ICC profile: this is a wide-gamut RGB color space
    } else if (output_type == DCM_ICC_OUTPUT_DISPLAY_P3) {
        // D65 White Point
        cmsCIExyY D65 = { 0.3127, 0.3290, 1.0 };
        // Display-P3 primaries (same as DCI-P3)
        cmsCIExyYTRIPLE DisplayP3Primaries = {
            {0.6800, 0.3200, 1.0}, // Display P3 Red
            {0.2650, 0.6900, 1.0}, // Display P3 Green
            {0.1500, 0.0600, 1.0}  // Display P3 Blue
        };
        // sRGB transfer function
        cmsToneCurve* sRGBTransferFunction[3];
        cmsFloat64Number Parameters[5];
        Parameters[0] = 2.4;
        Parameters[1] = 1. / 1.055;
        Parameters[2] = 0.055 / 1.055;
        Parameters[3] = 1. / 12.92;
        Parameters[4] = 0.04045;

        sRGBTransferFunction[0] = sRGBTransferFunction[1] = sRGBTransferFunction[2] = cmsBuildParametricToneCurve(NULL, 4, Parameters);
        if (sRGBTransferFunction[0] == NULL) {
            cmsCloseProfile(in_handle);
            return NULL;
        }

        // Combine the white point, primaries, and transfer functions into an RGB profile
        out_handle = cmsCreateRGBProfileTHR(NULL, &D65, &DisplayP3Primaries, sRGBTransferFunction);
        cmsFreeToneCurve(sRGBTransferFunction[0]);

        if (out_handle == NULL) {
            cmsCloseProfile(in_handle);
            return NULL;
        }

        // Set the profile description
        const wchar_t* description = L"Display-P3";
        cmsMLU* mlu = cmsMLUalloc(NULL, 1);
        cmsMLUsetWide(mlu, "en", "US", description);
        cmsWriteTag(out_handle, cmsSigProfileDescriptionTag, mlu);
        cmsMLUfree(mlu);
 
    } else {
        // Unknown or unsupported output_type
        cmsCloseProfile(in_handle);
        return NULL;
    }

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
    icc_transform->number_of_pixels = (uint32_t)rows * (uint32_t)columns;

    return icc_transform;
}

// Backward-compatible wrapper function
DmcIccTransform *dcm_icc_transform_create(const char *icc_profile,
                                          uint32_t icc_profile_size,
                                          uint8_t planar_configuration,
                                          uint16_t columns,
                                          uint16_t rows) {
    // Call the extended function with DCM_ICC_OUTPUT_SRGB as the default output type
    return dcm_icc_transform_create_for_output(icc_profile,
                                               icc_profile_size,
                                               planar_configuration,
                                               columns,
                                               rows,
                                               DCM_ICC_OUTPUT_SRGB);
}

void dcm_icc_transform_apply(const DmcIccTransform *icc_transform,
                             const char *frame,
                             uint32_t frame_size,
                             char *corrected_frame) {
    cmsDoTransform(icc_transform->handle,
                   frame,
                   corrected_frame,
                   icc_transform->number_of_pixels);
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
