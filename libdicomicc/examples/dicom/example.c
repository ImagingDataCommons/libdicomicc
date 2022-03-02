#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dicom.h>

#include "icc.h"

int main(int argc, char *argv[])
{
    char *file_path = "image.dcm";
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
    DcmElement *optical_path_element = dcm_dataset_get(metadata,
                                                       0x00480105);
    DcmSequence *optical_path_seq = dcm_element_get_value_SQ(optical_path_element);
    DcmDataSet *optical_path_item = dcm_sequence_get(optical_path_seq, 0);
    DcmElement *icc_profile_element = dcm_dataset_get(optical_path_item,
                                                      0x00282000);
    uint32_t icc_profile_length = dcm_element_get_length(icc_profile_element);
    const char *icc_profile = dcm_element_get_value_OB(icc_profile_element);
    DcmElement *planar_config_element = dcm_dataset_get(metadata,
                                                        0x00280006);
    uint8_t planar_config = dcm_element_get_value_US(planar_config_element, 0);

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
    uint16_t columns = dcm_frame_get_columns(frame);
    uint16_t rows = dcm_frame_get_rows(frame);

    dcm_log_info("Create ICC transform.");
    DmcIccTransform *icc_transform = dcm_icc_transform_create(icc_profile,
                                                              icc_profile_length,
                                                              planar_config,
                                                              columns,
                                                              rows);

    char *corrected_frame_value = malloc(frame_length);
    //this should be frame_length+1, but in this case we will have memory leaks
    if (corrected_frame_value == NULL) {
        dcm_log_error("Failed to allocate memory for frame buffer.");
        dcm_bot_destroy(bot);
        dcm_frame_destroy(frame);
        dcm_dataset_destroy(metadata);
        dcm_file_destroy(file);
        return EXIT_FAILURE;
    }

    dcm_log_info("Apply ICC transform to frame #%u", frame_number);
    dcm_icc_transform_apply(icc_transform,
                            frame_value,
                            corrected_frame_value);

    /*dcm_log_info("Check results");
    for (int i = 0; i < (int) frame_length / 3; i++) {
      dcm_log_info("Output - Input RGB first pixel: %u %u %u",
             (u_int8_t)corrected_frame_value[i*3] - (u_int8_t)frame_value[i*3],
             (u_int8_t)corrected_frame_value[i*3 + 1] - (u_int8_t)frame_value[i*3 + 1],
             (u_int8_t)corrected_frame_value[i*3 + 2] - (u_int8_t)frame_value[i*3 + 2]);
    }*/

    // print to file
    /*FILE *f = fopen("corrected_frame_value.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < (int) frame_length / 3; i++) {
      fprintf(f, "%f %f %f 1\n", ((u_int8_t)corrected_frame_value[i*3]) / 255.,
              ((u_int8_t)corrected_frame_value[i*3 + 1]) / 255.,
              ((u_int8_t)corrected_frame_value[i*3 + 2]) / 255.);
    }

    fclose(f);
    f = NULL;*/

    // clean
    dcm_log_info("Cleanup DICOM bot.");
    dcm_bot_destroy(bot);
    dcm_log_info("Cleanup DICOM frame.");
    dcm_frame_destroy(frame);
    dcm_log_info("Cleanup DICOM metadata.");
    dcm_dataset_destroy(metadata);
    dcm_log_info("Cleanup DICOM file.");
    dcm_file_destroy(file);

    dcm_log_info("Cleanup ICC transform.");
    dcm_icc_transform_destroy(icc_transform);

    dcm_log_info("Cleanup output image.");
    free(corrected_frame_value);

    return EXIT_SUCCESS;
}
