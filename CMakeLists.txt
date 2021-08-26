set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -v -DCMAKE_EXPORT_COMPILE_COMMANDS=True")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -v -DCMAKE_EXPORT_COMPILE_COMMANDS=True")
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(project VERSION 1.0.0
  DESCRIPTION project
  LANGUAGES CXX
  ) 
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
FIND_PACKAGE(Python3)
set(PYTHON ${Python3_EXECUTABLE} CACHE STRING "Python path")
set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten CACHE STRING "Emscripten path")
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)
set(MY_LIB_FRONT_MATTER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/front-matter.js)
set(MY_LIB_HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/myLib.h)
set(MY_LIB_IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/webidl/myLib.idl)
set(MY_LIB_ONLOAD_FILE ${CMAKE_CURRENT_SOURCE_DIR}/onload.js)
set(BULLET_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(TARGET_NAME project)
add_executable(${TARGET_NAME})
add_subdirectory(src)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${TARGET_NAME} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


target_link_libraries(${TARGET_NAME} PRIVATE
  ${PROJECT_WARNINGS}
  ${PROJECT_OPTIONS}
  CONAN_PKG::boost
)

# Build Release by default
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type")

# Default is 64*1024*1024 = 64MB
set(TOTAL_MEMORY 67108864 CACHE STRING "Total Memory")

# Enable for resizable heap, with some amount of slowness
set(ALLOW_MEMORY_GROWTH 0 CACHE STRING "Allow Memory Growth")

set(EMCC_ARGS
  --extern-pre-js ${MY_LIB_FRONT_MATTER_FILE}
  --llvm-lto 1
  --post-js glue.js
  --post-js ${MY_LIB_ONLOAD_FILE}
  -O3
  -s ALLOW_MEMORY_GROWTH=${ALLOW_MEMORY_GROWTH}
  -s ALLOW_TABLE_GROWTH=1
  -s EXPORTED_FUNCTIONS=["_malloc"]
  -s EXPORTED_RUNTIME_METHODS=["UTF8ToString"]
  -s EXTRA_EXPORTED_RUNTIME_METHODS=["addFunction"]
  -s EXPORT_NAME="MyLib"
  -s MODULARIZE=1
  -s NO_EXIT_RUNTIME=1
  -s NO_FILESYSTEM=1
  -s TOTAL_MEMORY=${TOTAL_MEMORY}
  )

if(${CLOSURE})
  # Ignore closure errors about the bullet Node class
  # (Node is a DOM thing too)
  LIST(APPEND EMCC_ARGS
    --closure 1
    -s IGNORE_CLOSURE_COMPILER_ERRORS=1)
else()
  LIST(APPEND EMCC_ARGS
    -s NO_DYNAMIC_EXECUTION=1)
endif()

set(EMCC_JS_ARGS ${EMCC_ARGS}
  -s AGGRESSIVE_VARIABLE_ELIMINATION=1
  -s ELIMINATE_DUPLICATE_FUNCTIONS=1
  -s LEGACY_VM_SUPPORT=1
  -s SINGLE_FILE=1
  -s WASM=0)

set(EMCC_WASM_ARGS ${EMCC_ARGS}
  -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1
  -s WASM=1
  -s LLD_REPORT_UNDEFINED
  )

set(EMCC_GLUE_ARGS
  -c
  -I${BULLET_SRC_DIR} 
  -I${CONAN_INCLUDE_DIRS}
  -include${MY_LIB_HEADER_FILE}
  )
  

#######################################
project("myLib")



#######################################
add_custom_command(
  OUTPUT glue.cpp glue.js
  BYPRODUCTS parser.out WebIDLGrammar.pkl
  COMMAND ${PYTHON} ${WEBIDL_BINDER_SCRIPT} ${MY_LIB_IDL_FILE} glue
  DEPENDS ${MY_LIB_IDL_FILE}
  COMMENT "Generating myLib bindings"
  VERBATIM)
add_custom_command(
  OUTPUT glue.o
  COMMAND emcc glue.cpp ${EMCC_GLUE_ARGS} -o glue.o
  DEPENDS glue.cpp ${MY_LIB_HEADER_FILE}
  COMMENT "Building myLib bindings"
  VERBATIM)
add_custom_target(myLib-bindings ALL DEPENDS glue.js glue.o)


#######################################
add_custom_command(
  OUTPUT myLib.js
  COMMAND emcc glue.o  ${BULLET_LIBS} ${EMCC_JS_ARGS} -o myLib.js
  DEPENDS myLib-bindings ${MY_LIB_FRONT_MATTER_FILE} ${MY_LIB_ONLOAD_FILE} ${TARGET_NAME}
  COMMENT "Building myLib javascript"
  VERBATIM)
add_custom_target(myLib-javascript ALL DEPENDS myLib.js)


#######################################
add_custom_command(
  OUTPUT myLib.wasm.js myLib.wasm.wasm
  COMMAND emcc glue.o  ${BULLET_LIBS} ${EMCC_WASM_ARGS} -o myLib.wasm.js
  DEPENDS myLib-bindings ${MY_LIB_FRONT_MATTER_FILE} ${MY_LIB_ONLOAD_FILE} ${TARGET_NAME}
  COMMENT "Building myLib webassembly"
  VERBATIM)
add_custom_target(myLib-wasm ALL DEPENDS myLib.wasm.js myLib.wasm.wasm)
