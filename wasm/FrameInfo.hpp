#pragma once

struct FrameInfo {
    /// <summary>
    /// Width of the image, range [1, 65535].
    /// </summary>
    uint16_t width;

    /// <summary>
    /// Height of the image, range [1, 65535].
    /// </summary>
    uint16_t height;

    /// <summary>
    /// Number of bits per sample, range [8, 32]
    /// </summary>
    uint8_t bitsPerSample;

    /// <summary>
    /// Number of components contained in the frame, range [1, 255]
    /// </summary>
    uint8_t componentCount;

    /// <summary>
    /// Planar configuration
    /// </summary>
    uint8_t planarConfiguration;
};
