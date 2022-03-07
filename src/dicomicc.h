#include <stdint.h>

#ifndef DCM_ICC_INCLUDED
#define DCM_ICC_INCLUDED

typedef struct _DmcIccTransform DmcIccTransform;

extern const char *dcm_icc_get_version(void);

extern DmcIccTransform *dcm_icc_transform_create(const char *icc_profile,
                                                 uint32_t icc_profile_size,
                                                 uint8_t planar_configuration,
                                                 uint16_t columns,
                                                 uint16_t rows);

extern void dcm_icc_transform_apply(const DmcIccTransform *icc_transform,
                                    const char *frame,
                                    uint32_t frame_size,
                                    char *corrected_frame);

extern void dcm_icc_transform_destroy(DmcIccTransform *icc_transform);

#endif
