cmake_minimum_required(VERSION 2.8)

project("Clang testiranje")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
)

set(CMAKE_C_COMPILER gcc-10)
set(CMAKE_CXX_COMPILER g++-10)

set(LLVM_VERSION 12)
set(CLANG_VERSION 12)

add_definitions(-DCLANG_VERSION=${CLANG_VERSION} -DLLVM_VERSION=${LLVM_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

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
  ../trans/header
)

link_directories(${LibClang_LINK_PATH})

find_package(Qt5Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

file(GLOB trans_sources "../trans/source/*.cpp")
file(GLOB trans_headers "../trans/header/*.hpp")

add_executable(trans_test main.cpp ${trans_sources} ${trans_headers})

add_test(NAME trans_test COMMAND trans_test)

target_link_libraries(trans_test libLLVM-${LLVM_VERSION}.so
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
                           libclangTooling.a
                           Qt5::Test)
