set (LIBDICOMICC_INCLUDE
  ${CMAKE_CURRENT_SOURCE_DIR}/../libdicomicc/src
  ${CMAKE_CURRENT_SOURCE_DIR}/../libdicomicc/libdicom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../libdicomicc/lcms2/include)

include_directories(${LIBDICOMICC_INCLUDE})

add_executable(libiccjs jslib.cpp)

target_link_libraries(libiccjs dicomicc)

set_target_properties(
    libiccjs
    PROPERTIES
    LINK_FLAGS "\
      --bind \
      -s MODULARIZE=1 \
      -s EXPORT_NAME=libicc \
      -s WASM=0 \
      -s DISABLE_EXCEPTION_CATCHING=1 \
      -s ASSERTIONS=0 \
      -s NO_EXIT_RUNTIME=1 \
      -s MALLOC=emmalloc \
      -s ALLOW_MEMORY_GROWTH=1 \
      -s TOTAL_MEMORY=1073741824 \
      -s FILESYSTEM=0 \
      -s EXPORTED_FUNCTIONS=[] \
      -s EXPORTED_RUNTIME_METHODS=[ccall] \
      -s LLD_REPORT_UNDEFINED \
   ")

# WASM BUILD
add_executable(libiccwasm jslib.cpp)

target_link_libraries(libiccwasm dicomicc)

set_target_properties(
  libiccwasm
    PROPERTIES
    LINK_FLAGS "\
      --bind \
      -s MODULARIZE=1 \
      -s EXPORT_NAME=libiccWASM \
      -s WASM=1 \
      -s DISABLE_EXCEPTION_CATCHING=1 \
      -s ASSERTIONS=0 \
      -s NO_EXIT_RUNTIME=1 \
      -s MALLOC=emmalloc \
      -s ALLOW_MEMORY_GROWTH=1 \
      -s TOTAL_MEMORY=1073741824 \
      -s FILESYSTEM=0 \
      -s EXPORTED_FUNCTIONS=[] \
      -s EXPORTED_RUNTIME_METHODS=[ccall] \
      -s LLD_REPORT_UNDEFINED \
   ")

