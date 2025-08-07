#include "ColorManager.hpp"

#include <emscripten.h>
#include <emscripten/bind.h>

extern "C" {
  #include <dicomicc.h>
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(DcmIccOutputType) {
  enum_<DcmIccOutputType>("DcmIccOutputType")
    .value("SRGB", DCM_ICC_OUTPUT_SRGB)
    .value("DISPLAY_P3", DCM_ICC_OUTPUT_DISPLAY_P3)
    .value("ADOBE_RGB", DCM_ICC_OUTPUT_ADOBE_RGB)
    .value("ROMM_RGB", DCM_ICC_OUTPUT_ROMM_RGB)
  ;
}

EMSCRIPTEN_BINDINGS(FrameInfo) {
  value_object<FrameInfo>("FrameInfo")
    .field("columns", &FrameInfo::columns)
    .field("rows", &FrameInfo::rows)
    .field("bitsPerSample", &FrameInfo::bitsPerSample)
    .field("samplesPerPixel", &FrameInfo::samplesPerPixel)
    .field("planarConfiguration", &FrameInfo::planarConfiguration)
  ;
}

EMSCRIPTEN_BINDINGS(ColorManager) {
  class_<ColorManager>("ColorManager")
    .constructor<FrameInfo, const val, int>()
    .function("getFrameInfo", &ColorManager::getFrameInfo)
    .function("transform", &ColorManager::transform)
  ;
}


std::string getExceptionMessage(intptr_t exceptionPtr) {
  return std::string(reinterpret_cast<std::exception *>(exceptionPtr)->what());
}

EMSCRIPTEN_BINDINGS(dicomicc) {
  emscripten::function("getExceptionMessage", &getExceptionMessage);
}
