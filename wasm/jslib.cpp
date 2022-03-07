#include "ICCCMSTransform.hpp"

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(FrameInfo) {
  value_object<FrameInfo>("FrameInfo")
    .field("width", &FrameInfo::width)
    .field("height", &FrameInfo::height)
    .field("bitsPerSample", &FrameInfo::bitsPerSample)
    .field("componentCount", &FrameInfo::componentCount)
    .field("planarConfiguration", &FrameInfo::planarConfiguration)
  ;
}

EMSCRIPTEN_BINDINGS(ICCCMSTransform) {
  class_<ICCCMSTransform>("ICCCMSTransform")
    .constructor<>()
    .function("getInputBuffer", &ICCCMSTransform::getInputBuffer)
    .function("getICCBuffer", &ICCCMSTransform::getICCBuffer)
    .function("getOutputBuffer", &ICCCMSTransform::getOutputBuffer)
    .function("transform", &ICCCMSTransform::transform)
    .function("getFrameInfo", &ICCCMSTransform::getFrameInfo)
    .function("setFrameInfo", &ICCCMSTransform::setFrameInfo)
  ;
}


std::string getExceptionMessage(intptr_t exceptionPtr) {
  return std::string(reinterpret_cast<std::exception *>(exceptionPtr)->what());
}

EMSCRIPTEN_BINDINGS(ICCCMS) {
  emscripten::function("getExceptionMessage", &getExceptionMessage);
  // function("doLeakCheck", &__lsan_do_recoverable_leak_check);
}
