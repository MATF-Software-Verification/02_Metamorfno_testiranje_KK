cmake_minimum_required(VERSION 2.8)

project("Clang testiranje")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
)

set(CMAKE_C_COMPILER clang-11)
set(CMAKE_CXX_COMPILER clang++-11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")

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
  ../trans/header
)

link_directories(${LibClang_LINK_PATH})

find_package(Qt5Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_executable(test main.cpp
                    ../trans/header/MTKConsumer.hpp
                    ../trans/header/MTKContext.hpp
                    ../trans/source/MTKContext.cpp
                    ../trans/header/MTKTransformer.hpp
                    ../trans/source/MTKTransformer.cpp
                    ../trans/header/Do2ForVisitor.hpp
                    ../trans/source/Do2ForVisitor.cpp
                    ../trans/header/For2DoVisitor.hpp
                    ../trans/source/For2DoVisitor.cpp
                    ../trans/header/For2WhileVisitor.hpp
                    ../trans/source/For2WhileVisitor.cpp
                    ../trans/header/While2DoVisitor.hpp
                    ../trans/source/While2DoVisitor.cpp
                    ../trans/header/While2ForVisitor.hpp
                    ../trans/source/While2ForVisitor.cpp
                    ../trans/header/PrepForVisitor.hpp
                    ../trans/source/PrepForVisitor.cpp
                    ../trans/header/PrepIfVisitor.hpp
                    ../trans/source/PrepIfVisitor.cpp
                    ../trans/header/PrepSwitchVisitor.hpp
                    ../trans/source/PrepSwitchVisitor.cpp
                    ../trans/header/If2SwitchVisitor.hpp
                    ../trans/source/If2SwitchVisitor.cpp
                    ../trans/header/Switch2IfVisitor.hpp
                    ../trans/source/Switch2IfVisitor.cpp
                    ../trans/header/Rek2IterVisitor.hpp
                    ../trans/source/Rek2IterVisitor.cpp
                    ../trans/header/FinIterVisitor.hpp
                    ../trans/source/FinIterVisitor.cpp
                    ../trans/header/Iter2RekVisitor.hpp
                    ../trans/source/Iter2RekVisitor.cpp
                    ../trans/header/FinRekVisitor.hpp
                    ../trans/source/FinRekVisitor.cpp
                    ../trans/header/LoopUnrollVisitor.hpp
                    ../trans/source/LoopUnrollVisitor.cpp
                    ../trans/header/CodeImputVisitor.hpp
                    ../trans/source/CodeImputVisitor.cpp)

add_test(NAME test COMMAND test)

target_link_libraries(test libLLVM-11.so
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
