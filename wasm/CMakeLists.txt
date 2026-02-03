include_directories(${DICOMICC_INCLUDE_DIR} ${LCMS2_INCLUDE_DIR})

# WASM BUILD
add_executable(dicomiccwasm src/jslib.cpp)
target_link_libraries(dicomiccwasm ${DICOMICC_LIBRARY})
set_target_properties(dicomiccwasm PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
set_target_properties(
  dicomiccwasm
    PROPERTIES
    LINK_FLAGS "\
      --bind \
      -s MODULARIZE=1 \
      -s EXPORT_NAME=dicomicc \
      -s WASM=1 \
      -s DISABLE_EXCEPTION_CATCHING=1 \
      -s ASSERTIONS=0 \
      -s NO_EXIT_RUNTIME=1 \
      -s MALLOC=emmalloc \
      -s ALLOW_MEMORY_GROWTH=1 \
      -s TOTAL_MEMORY=1073741824 \
      -s FILESYSTEM=0 \
      -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
      -s EXPORTED_FUNCTIONS=[] \
      -s EXPORTED_RUNTIME_METHODS=[ccall] \
   ")

