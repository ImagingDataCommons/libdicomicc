#include <stdint.h>

#ifndef ICC_INCLUDED
#define ICC_INCLUDED

typedef struct _iccTransform iccTransform;

extern const char *icc_get_version(void);

extern iccTransform *icc_transform_create(const char *icc_profile,
                                          uint32_t icc_profile_size,
                                          uint8_t planar_configuration);

extern void icc_transform_apply(const iccTransform *icc_transform,
                                const char *frame,
                                uint32_t numer_of_pixels,
                                char *corrected_frame);

extern void icc_transform_destroy(iccTransform *icc_transform);

extern uint32_t icc_calculate_image_size(uint32_t frame_length,
                                         uint16_t samples_per_pixel);

#endif
