#pragma once

extern "C" {
  #include <dicomicc.h>
}
#include <vector>
#include <emscripten/val.h>

#include "FrameInfo.hpp"

using namespace emscripten;

thread_local const val Uint8ClampedArray = val::global("Uint8ClampedArray");

/// <summary>
/// JavaScript API for applying ICC color profiles.
/// </summary>
class ColorManager {
  public: 
  /// <summary>
  /// Constructor
  /// </summary>
  ColorManager(FrameInfo frameInfo,
               const val &iccProfile,
               int outputType = DCM_ICC_OUTPUT_SRGB) {

    this->frameInfo = frameInfo;

    const std::vector<uint8_t> iccProfileVector =
      convertJSArrayToNumberVector<uint8_t>(iccProfile);

    this->icc_transform = dcm_icc_transform_create_for_output((const char *) iccProfileVector.data(),
                                                              (uint32_t) iccProfileVector.size(),
                                                              this->frameInfo.planarConfiguration,
                                                              this->frameInfo.columns,
                                                              this->frameInfo.rows,
                                                              static_cast<DcmIccOutputType>(outputType));
  }

  /// <summary>
  /// Destructor
  /// </summary>
  ~ColorManager() {
    dcm_icc_transform_destroy(icc_transform);
  }
  
  /// <summary>
  /// Apply ICC color profiles to the input bitstream.
  ///
  /// Returns a TypedArray of the buffer with the resulting pixel data.
  /// </summary>
  val transform(const val &inputFrame) {

    const std::vector<uint8_t> inputFrameVector =
      convertJSArrayToNumberVector<uint8_t>(inputFrame);

    this->output.resize(inputFrameVector.size());

    dcm_icc_transform_apply(this->icc_transform,
                            (const char *) inputFrameVector.data(),
                            (uint32_t) inputFrameVector.size(),
                            (char *) this->output.data());

    // Create a JavaScript-friendly result from the memory view
    // instead of relying on the consumer to detach it from WASM memory
    // See https://web.dev/webassembly-memory-debugging/
    val js_result = Uint8ClampedArray.new_(typed_memory_view(
      this->output.size(), this->output.data()
    ));

    return js_result;
  }

  /// <summary>
  /// returns the FrameInfo object.
  /// </summary>
  const FrameInfo& getFrameInfo() const {
      return this->frameInfo;
  }

  private:
    std::vector<uint8_t> output;
    FrameInfo frameInfo;
    DmcIccTransform *icc_transform;
};
