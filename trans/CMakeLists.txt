cmake_minimum_required(VERSION 2.8)

project("Clang transformacije")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
)

set(LLVM_VERSION 12)
set(CLANG_VERSION 12)

add_definitions(-DCLANG_VERSION=${CLANG_VERSION} -DLLVM_VERSION=${LLVM_VERSION})

set(CMAKE_C_COMPILER clang-${CLANG_VERSION})
set(CMAKE_CXX_COMPILER clang++-${CLANG_VERSION})

set(CMAKE_CXX_STANDARD 17)
execute_process(
  COMMAND llvm-config-${LLVM_VERSION} --cxxflags
  OUTPUT_VARIABLE LibClang_DEFINITIONS)

set(LibClang_DEFINITIONS ${LibClang_DEFINITIONS} "-fno-rtti")

execute_process(
  COMMAND llvm-config-${LLVM_VERSION} --includedir
  OUTPUT_VARIABLE LibClang_INCLUDE_DIR)

set(LibClang_LIBRARY /usr/lib/llvm-${LLVM_VERSION}/lib/libclang.so)
set(LibClang_LINK_PATH /usr/lib/llvm-${LLVM_VERSION}/lib)

find_library(LibClang_LIBRARY NAMES clang)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibClang DEFAULT_MSG LibClang_LIBRARY LibClang_INCLUDE_DIR)

mark_as_advanced(LibClang_INCLUDE_DIR LibClang_LIBRARY)

add_definitions(${LibClang_DEFINITIONS})

include_directories(
  ${LibClang_INCLUDE_DIR}
  header
)

link_directories(${LibClang_LINK_PATH})

file(GLOB trans_code "header/*.hpp" "source/*.cpp" "header/*.h")
add_executable(trans ${trans_code})

target_link_libraries(trans libLLVM-${LLVM_VERSION}.so
                            libclangTooling.a
                            libclangAnalysis.a
                            libclangAST.a
                            libclangBasic.a
                            libclangDriver.a
                            libclangEdit.a
                            libclangFrontend.a
                            libclangLex.a
                            libclangParse.a
                            libclangRewrite.a
                            libclangSema.a
                            libclangSerialization.a
                            libclangTooling.a)
                        if(CMAKE_BUILD_TYPE MATCHES Debug)
                            add_definitions(-DDEBUG)
                        endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions(-DENABLE_ASSERT)
