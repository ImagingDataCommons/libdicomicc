#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dicom.h>

#include "icc.h"

int main(int argc, char *argv[])
{
    const char *file_path = "image.dcm";
    uint32_t frame_number = 1;
    dcm_log_level = DCM_LOG_INFO;

    if (argc != 2) {
        dcm_log_error("Path to DICOM file must be provided.");
        return EXIT_FAILURE;
    }
    file_path = argv[1];

    DcmFile *file = dcm_file_create(file_path, 'r');
    if (file == NULL) {
        dcm_log_error("Reading DICOM file '%s' failed.", file_path);
        return EXIT_FAILURE;
    }

    dcm_log_info("Read metadata from DICOM file.");
    DcmDataSet *metadata = dcm_file_read_metadata(file);
    if (metadata == NULL) {
        dcm_log_error("Reading DICOM file '%s' failed. "
                      "Could not read metadata.",
                      file_path);
        dcm_file_destroy(file);
        return EXIT_FAILURE;
    }

    dcm_log_info("Get ICC profile");
    const DcmElement *optical_path_element = dcm_dataset_get(metadata,
                                                             0x00480105);
    const DcmSequence *optical_path_seq = dcm_element_get_value_SQ(optical_path_element);
    const DcmDataSet *optical_path_item = dcm_sequence_get(optical_path_seq, 0);
    const DcmElement *icc_profile_element = dcm_dataset_get(optical_path_item,
                                                            0x00282000);
    uint32_t icc_profile_length = dcm_element_get_length(icc_profile_element);
    const char *icc_profile = dcm_element_get_value_OB(icc_profile_element);
    const DcmElement *planar_config_element = dcm_dataset_get(metadata,
                                                              0x00280006);
    uint8_t planar_config = dcm_element_get_value_US(planar_config_element, 0);

    dcm_log_info("Create ICC transform.");
    const iccTransform *icc_transform = icc_transform_create(icc_profile,
                                                             icc_profile_length,
                                                             planar_config);

    dcm_log_info("Read frame #%u from DICOM file.", frame_number);
    DcmBOT *bot = dcm_file_build_bot(file, metadata);
    DcmFrame *frame = dcm_file_read_frame(file, metadata, bot, frame_number);
    if (frame == NULL) {
        dcm_log_error("Reading DICOM file '%s' failed. "
                      "Could not read frame #%u.",
                      file_path, frame_number);
        dcm_bot_destroy(bot);
        dcm_dataset_destroy(metadata);
        dcm_file_destroy(file);
        return EXIT_FAILURE;
    }
    const char *frame_value = dcm_frame_get_value(frame);
    uint32_t frame_length = dcm_frame_get_length(frame);

    char *corrected_frame_value = malloc(frame_length);
    if (corrected_frame_value == NULL) {
        dcm_log_error("Failed to allocate memory for frame buffer.");
        dcm_bot_destroy(bot);
        dcm_frame_destroy(frame);
        dcm_dataset_destroy(metadata);
        dcm_file_destroy(file);
        return EXIT_FAILURE;
    }

    dcm_log_info("Apply ICC transform to frame #%u", frame_number);
    icc_transform_apply(icc_transform,
                        frame_value,
                        frame_length,
                        corrected_frame_value);

    dcm_log_info("Cleanup DICOM file.");
    dcm_bot_destroy(bot);
    dcm_frame_destroy(frame);
    dcm_dataset_destroy(metadata);
    dcm_file_destroy(file);
    free(corrected_frame_value);

    dcm_log_info("Cleanup ICC transform.");
    icc_transform_destroy(icc_transform);

    return EXIT_SUCCESS;
}
