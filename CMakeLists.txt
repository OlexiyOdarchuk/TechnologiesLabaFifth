cmake_minimum_required(VERSION 3.15)
project(Validator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)

file(GLOB SOURCES src/*.cpp)
file(GLOB SIMPLE Laba_5.cpp)
file(GLOB ONEFILE Laba_5_onefile.cpp)

add_executable(Validator ${SOURCES})
add_executable(Simple_Validator ${SIMPLE})
add_executable(OneFile_Validator ${ONEFILE})
