cmake_minimum_required(VERSION 3.5)
project(Compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


include_directories(include)
set(SOURCE_FILES src/main.cpp src/preprocessor.cpp src/misc.cpp src/compiler.cpp)
add_executable(Compiler ${SOURCE_FILES})