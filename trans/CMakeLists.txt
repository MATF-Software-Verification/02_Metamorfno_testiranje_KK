cmake_minimum_required(VERSION 2.8)

project("Clang transformacije")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
)

set(CMAKE_C_COMPILER clang-11)
set(CMAKE_CXX_COMPILER clang++-11)

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
  header
)

link_directories(${LibClang_LINK_PATH})

add_executable(trans source/main.cpp
                     header/MTKConsumer.hpp
                     header/MTKContext.hpp
                     source/MTKContext.cpp
                     header/MTKTransformer.hpp
                     source/MTKTransformer.cpp
                     header/Do2ForVisitor.hpp
                     source/Do2ForVisitor.cpp
                     header/For2DoVisitor.hpp
                     source/For2DoVisitor.cpp
                     header/For2WhileVisitor.hpp
                     source/For2WhileVisitor.cpp
                     header/While2DoVisitor.hpp
                     source/While2DoVisitor.cpp
                     header/While2ForVisitor.hpp
                     source/While2ForVisitor.cpp
                     header/PrepForVisitor.hpp
                     source/PrepForVisitor.cpp
                     header/PrepIfVisitor.hpp
                     source/PrepIfVisitor.cpp
                     header/PrepSwitchVisitor.hpp
                     source/PrepSwitchVisitor.cpp
                     header/If2SwitchVisitor.hpp
                     source/If2SwitchVisitor.cpp
                     header/Switch2IfVisitor.hpp
                     source/Switch2IfVisitor.cpp
                     header/Rek2IterVisitor.hpp
                     source/Rek2IterVisitor.cpp
                     header/FinIterVisitor.hpp
                     source/FinIterVisitor.cpp
                     header/Iter2RekVisitor.hpp
                     source/Iter2RekVisitor.cpp
                     header/FinRekVisitor.hpp
                     source/FinRekVisitor.cpp
                     header/LoopUnrollVisitor.hpp
                     source/LoopUnrollVisitor.cpp
                     header/CodeImputVisitor.hpp
                     source/CodeImputVisitor.cpp)

target_link_libraries(trans libLLVM-11.so
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
