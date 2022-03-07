#pragma once

struct FrameInfo {
    /// <summary>
    /// Width of the image, range [1, 65535].
    /// </summary>
    uint16_t columns;

    /// <summary>
    /// Height of the image, range [1, 65535].
    /// </summary>
    uint16_t rows;

    /// <summary>
    /// Number of bits per sample, range [8, 32].
    /// </summary>
    uint8_t bitsPerSample;

    /// <summary>
    /// Number of components contained in the frame.
    /// </summary>
    uint8_t samplesPerPixel;

    /// <summary>
    /// Planar configuration
    /// </summary>
    uint8_t planarConfiguration;
};
