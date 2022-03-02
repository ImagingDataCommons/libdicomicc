#pragma once

extern "C" {
  #include <icc.h>
}
#include <vector>
#include <emscripten/val.h>

#include "FrameInfo.hpp"

using namespace emscripten;

thread_local const val Uint8ClampedArray = val::global("Uint8ClampedArray");

/// <summary>
/// JavaScript API for applying ICC color profiles.
/// </summary>
class ICCCMSTransform {
  public: 
  /// <summary>
  /// Constructor
  /// </summary>
  ICCCMSTransform() {
  }

  /// <summary>
  /// Resizes encoded buffer and returns a TypedArray of the buffer allocated
  /// in WASM memory space that will hold the bitstream.
  /// JavaScript code needs to copy the bistream into the
  /// returned TypedArray. This copy operation is needed because WASM runs
  /// in a sandbox and cannot access memory managed by JavaScript.
  /// </summary>
  val getInputBuffer(size_t inputSize) {
    input_.resize(inputSize);

    return emscripten::val(emscripten::typed_memory_view(input_.size(), input_.data()));
  }

  /// <summary>
  /// Resizes encoded buffer and returns a TypedArray of the buffer allocated
  /// in WASM memory space that will hold the icc profile bitstream.
  /// JavaScript code needs to copy the bistream into the
  /// returned TypedArray. This copy operation is needed because WASM runs
  /// in a sandbox and cannot access memory managed by JavaScript.
  /// </summary>
  val getICCBuffer(size_t iccProfileSize) {
    iccProfile_.resize(iccProfileSize);

    return emscripten::val(emscripten::typed_memory_view(iccProfile_.size(), iccProfile_.data()));
  }
  
  /// <summary>
  /// Returns a TypedArray of the buffer with the resulting pixel data
  /// </summary>
  val getOutputBuffer() {
    // Create a JavaScript-friendly result from the memory view
    // instead of relying on the consumer to detach it from WASM memory
    // See https://web.dev/webassembly-memory-debugging/
    val js_result = Uint8ClampedArray.new_(typed_memory_view(
      output_.size(), output_.data()
    ));
    
    return js_result;
  }

  /// <summary>
  /// Apply ICC color profiles to the input bitstream. The caller must have copied the
  /// bitstream into the buffer before calling this
  /// method, see getInputBuffer() above.
  /// </summary>
  void transform() {
    DmcIccTransform *icc_transform = dcm_icc_transform_create((const char *) iccProfile_.data(),
                                                              (uint32_t) iccProfile_.size(),
                                                              frameInfo_.planarConfiguration,
                                                              frameInfo_.width,
                                                              frameInfo_.height);
    output_.resize(input_.size());

    dcm_icc_transform_apply(icc_transform,
                            (const char *) input_.data(),
                            (char *) output_.data());

    dcm_icc_transform_destroy(icc_transform);
  }

  /// <summary>
  /// returns the FrameInfo object.
  /// </summary>
  const FrameInfo& getFrameInfo() const {
      return frameInfo_;
  }

  /// <summary>
  /// set image frame info
  /// </summary>
  void setFrameInfo(FrameInfo frameInfo) {
    frameInfo_ = frameInfo;
  }

  private:
    std::vector<uint8_t> input_;
    std::vector<uint8_t> iccProfile_;
    std::vector<uint8_t> output_;
    FrameInfo frameInfo_;
};
