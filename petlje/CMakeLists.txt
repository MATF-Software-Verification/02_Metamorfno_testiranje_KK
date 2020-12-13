cmake_minimum_required(VERSION 2.8)

project("Transformacije petlji")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
)

execute_process(
  COMMAND llvm-config-11 --cxxflags
  OUTPUT_VARIABLE LibClang_DEFINITIONS)

set(LibClang_DEFINITIONS ${LibClang_DEFINITIONS} "-fno-rtti")

execute_process(
  COMMAND llvm-config-11 --includedir
  OUTPUT_VARIABLE LibClang_INCLUDE_DIR)

set(LibClang_LIBRARY /usr/lib/llvm-11/lib/libclang.so)
set(LibClang_LINK_PATH /usr/lib/llvm-11/lib)

find_library(LibClang_LIBRARY NAMES clang)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibClang DEFAULT_MSG LibClang_LIBRARY LibClang_INCLUDE_DIR)

mark_as_advanced(LibClang_INCLUDE_DIR LibClang_LIBRARY)

add_definitions(${LibClang_DEFINITIONS})

include_directories(
  ${LibClang_INCLUDE_DIR}
  inc
)

add_executable(petlje src/petlje.cpp src/Do2ForConsumer.cpp src/For2DoConsumer.cpp src/For2WhileConsumer.cpp src/While2DoConsumer.cpp src/While2ForConsumer.cpp src/PrepForConsumer.cpp)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libLLVM-11.so)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangTooling.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangAnalysis.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangAST.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangBasic.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangDriver.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangEdit.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangFrontend.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangLex.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangParse.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangRewrite.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangSema.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangSerialization.a)
target_link_libraries(petlje ${LibClang_LINK_PATH}/libclangTooling.a)
