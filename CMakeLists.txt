cmake_minimum_required(VERSION 3.5)
project(Compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp preprocessor.cpp misc.cpp misc.h)
add_executable(Compiler ${SOURCE_FILES})