cmake_minimum_required(VERSION 2.6)
project(Silent)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories(${CMAKE_SOURCE_DIR}/SilentCompiler)
add_subdirectory(SilentCompiler)