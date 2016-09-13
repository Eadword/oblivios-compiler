cmake_minimum_required(VERSION 3.5)
project(Compiler)

find_package(BISON)
find_package(FLEX)

bison_target(LanguageParser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
flex_target(LanguageScanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
add_flex_bison_dependency(LanguageScanner LanguageParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


include_directories(include)
include_directories(data)
set(SOURCE_FILES src/main.cpp src/preprocessor.cpp src/misc.cpp src/compiler.cpp src/instruction.cpp src/numbers.cpp)
add_executable(
    Compiler
    ${SOURCE_FILES}
    ${BISON_LanguageParser_OUTPUTS}
    ${FLEX_LanguageScanner_OUTPUTS}
)
