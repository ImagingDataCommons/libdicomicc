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

/**
 * Create an sRGB ICC profile
 */
cmsHPROFILE create_srgb_profile(void) {
    // Use the built-in littleCMS sRGB profile creation function
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    
    if (profile == NULL) {
        fprintf(stderr, "Error: Failed to create sRGB profile\n");
        return NULL;
    }

    return profile;
}

/**
 * Create a Display-P3 ICC profile
 * Based on https://www.color.org/chardata/rgb/DisplayP3.xalter
 */
cmsHPROFILE create_display_p3_profile(void) {
    // D65 White Point
    cmsCIExyY D65 = { 0.3127, 0.3290, 1.0 };
    
    // Display-P3 primaries (same as DCI-P3)
    cmsCIExyYTRIPLE DisplayP3Primaries = {
        {0.6800, 0.3200, 1.0}, // Display P3 Red
        {0.2650, 0.6900, 1.0}, // Display P3 Green
        {0.1500, 0.0600, 1.0}  // Display P3 Blue
    };
    
    // sRGB transfer function parameters, parametric curve type 4:
    // Y = (a * X + b) ^ gamma for X >= d
    // Y =  c * X              for X <  d
    cmsToneCurve* sRGBTransferFunction[3];
    cmsFloat64Number Parameters[5];
    Parameters[0] = 2.4;           // Gamma
    Parameters[1] = 1. / 1.055;    // a
    Parameters[2] = 0.055 / 1.055; // b
    Parameters[3] = 1. / 12.92;    // c
    Parameters[4] = 0.04045;       // d

    // Create the tone curve
    sRGBTransferFunction[0] = sRGBTransferFunction[1] = sRGBTransferFunction[2] = 
        cmsBuildParametricToneCurve(NULL, 4, Parameters);
    
    if (sRGBTransferFunction[0] == NULL) {
        fprintf(stderr, "Error: Failed to create sRGB transfer function\n");
        return NULL;
    }

    // Combine the white point, primaries, and transfer functions into an RGB profile
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(NULL, &D65, &DisplayP3Primaries, sRGBTransferFunction);
    
    // Free the tone curve (profile now owns a copy)
    cmsFreeToneCurve(sRGBTransferFunction[0]);

    if (profile == NULL) {
        fprintf(stderr, "Error: Failed to create Display-P3 profile\n");
        return NULL;
    }

    // Set the profile description
    const wchar_t* description = L"Display-P3";
    cmsMLU* mlu = cmsMLUalloc(NULL, 1);
    if (mlu != NULL) {
        cmsMLUsetWide(mlu, "en", "US", description);
        cmsWriteTag(profile, cmsSigProfileDescriptionTag, mlu);
        cmsMLUfree(mlu);
    }

    // Set additional profile information
    const wchar_t* copyright = L"Public Domain";
    cmsMLU* copyright_mlu = cmsMLUalloc(NULL, 1);
    if (copyright_mlu != NULL) {
        cmsMLUsetWide(copyright_mlu, "en", "US", copyright);
        cmsWriteTag(profile, cmsSigCopyrightTag, copyright_mlu);
        cmsMLUfree(copyright_mlu);
    }

    return profile;
}

/**
 * Create an Adobe RGB ICC profile
 * Based on https://www.adobe.com/digitalimag/pdfs/AdobeRGB1998.pdf (incl. Annex C)
 */
cmsHPROFILE create_adobe_rgb_profile(void) {
    // D65 White Point
    cmsCIExyY D65 = { 0.3127, 0.3290, 1.0 };
    
    // AdobeRGB primaries
    cmsCIExyYTRIPLE AdobeRGBPrimaries = {
        {0.6400, 0.3300, 1.0}, // Adobe RGB Red
        {0.2100, 0.7100, 1.0}, // Adobe RGB Green
        {0.1500, 0.0600, 1.0}  // Adobe RGB Blue
    };
    
    // Adobe RGB transfer function parameters, parametric curve type 4:
    // Y = (a * X + b) ^ gamma for X >= d
    // Y =  c * X              for X <  d
    cmsToneCurve* AdobeRGBTransferFunction[3];
    cmsFloat64Number Parameters[5];
    Parameters[0] = 2. + 13107. / 65536.; // Gamma
    Parameters[1] = 1.;                   // a
    Parameters[2] = 0;                    // b
    Parameters[3] = 1. / 32;              // c
    Parameters[4] = 0.055680761;          // d (= c ^ (1 / (gamma - 1))

    // Create the tone curve
    AdobeRGBTransferFunction[0] = AdobeRGBTransferFunction[1] = AdobeRGBTransferFunction[2] = 
        cmsBuildParametricToneCurve(NULL, 4, Parameters);

    if (AdobeRGBTransferFunction[0] == NULL) {
        fprintf(stderr, "Error: Failed to create Adobe RGB transfer function\n");
        return NULL;
    }

    // Combine the white point, primaries, and transfer functions into an RGB profile
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(NULL, &D65, &AdobeRGBPrimaries, AdobeRGBTransferFunction);
    
    // Free the tone curve (profile now owns a copy)
    cmsFreeToneCurve(AdobeRGBTransferFunction[0]);

    if (profile == NULL) {
        fprintf(stderr, "Error: Failed to create Adobe RGB profile\n");
        return NULL;
    }

    // Set the profile description
    const wchar_t* description = L"Adobe RGB (1998)";
    cmsMLU* mlu = cmsMLUalloc(NULL, 1);
    if (mlu != NULL) {
        cmsMLUsetWide(mlu, "en", "US", description);
        cmsWriteTag(profile, cmsSigProfileDescriptionTag, mlu);
        cmsMLUfree(mlu);
    }

    // Set additional profile information
    const wchar_t* copyright = L"Public Domain";
    cmsMLU* copyright_mlu = cmsMLUalloc(NULL, 1);
    if (copyright_mlu != NULL) {
        cmsMLUsetWide(copyright_mlu, "en", "US", copyright);
        cmsWriteTag(profile, cmsSigCopyrightTag, copyright_mlu);
        cmsMLUfree(copyright_mlu);
    }

    return profile;
}

/*
 * Create a ROMM RGB ICC profile
 * Based on https://www.color.org/chardata/rgb/ROMMRGB.pdf
 */
cmsHPROFILE create_romm_rgb_profile(void) {
    // D50 White Point
    cmsCIExyY D50 = { 0.3457, 0.3585, 1.0 };
    
    // ROMM RGB primaries
    cmsCIExyYTRIPLE RommRGBPrimaries = {
        {0.7347, 0.2653, 1.0}, // ROMM RGB Red
        {0.1596, 0.8404, 1.0}, // ROMM RGB Green
        {0.0366, 0.0001, 1.0}  // ROMM RGB Blue
    };

    // ROMM RGB transfer function parameters, parametric curve type 5:
    // Y = (a * X + b) ^ gamma + e for X >= d
    // Y =  c * X              + f for X <  d
    cmsToneCurve* RommRGBTransferFunction[3];
    cmsFloat64Number Parameters[7];
    Parameters[0] = 1.8;                  // Gamma
    Parameters[1] = 0.996527;             // a
    Parameters[2] = 0.003473;             // b
    Parameters[3] = 0.0622829;            // c
    Parameters[4] = 0.03125;              // d
    Parameters[5] = 0.0;                  // e
    Parameters[6] = 0.003473;             // f

    // Create the tone curve
    RommRGBTransferFunction[0] = RommRGBTransferFunction[1] = RommRGBTransferFunction[2] = 
        cmsBuildParametricToneCurve(NULL, 5, Parameters);

    if (RommRGBTransferFunction[0] == NULL) {
        fprintf(stderr, "Error: Failed to create Romm RGB transfer function\n");
        return NULL;
    }

    // Combine the white point, primaries, and transfer functions into an RGB profile
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(NULL, &D50, &RommRGBPrimaries, RommRGBTransferFunction);
    
    // Free the tone curve (profile now owns a copy)
    cmsFreeToneCurve(RommRGBTransferFunction[0]);

    if (profile == NULL) {
        fprintf(stderr, "Error: Failed to create Romm RGB profile\n");
        return NULL;
    }

    // Set the profile description
    const wchar_t* description = L"ROMM RGB";
    cmsMLU* mlu = cmsMLUalloc(NULL, 1);
    if (mlu != NULL) {
        cmsMLUsetWide(mlu, "en", "US", description);
        cmsWriteTag(profile, cmsSigProfileDescriptionTag, mlu);
        cmsMLUfree(mlu);
    }

    // Set additional profile information
    const wchar_t* copyright = L"Public Domain";
    cmsMLU* copyright_mlu = cmsMLUalloc(NULL, 1);
    if (copyright_mlu != NULL) {
        cmsMLUsetWide(copyright_mlu, "en", "US", copyright);
        cmsWriteTag(profile, cmsSigCopyrightTag, copyright_mlu);
        cmsMLUfree(copyright_mlu);
    }

    return profile;
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

    switch (output_type) {
        case DCM_ICC_OUTPUT_SRGB:
            out_handle = create_srgb_profile();
            break;
        case DCM_ICC_OUTPUT_DISPLAY_P3:
            out_handle = create_display_p3_profile();
            break;
        case DCM_ICC_OUTPUT_ADOBE_RGB:
            out_handle = create_adobe_rgb_profile();
            break;
        case DCM_ICC_OUTPUT_ROMM_RGB:
            out_handle = create_romm_rgb_profile();
            break;
        default:
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
