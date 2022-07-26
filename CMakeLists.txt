cmake_minimum_required(VERSION 3.5)
project(ccanny)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wfatal-errors -Wno-unused-result\
                     -Wno-unknown-pragmas -fopenmp")

include_directories(${CMAKE_SOURCE_DIR}/3rd/)
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(ccanny main.cc ${SOURCES})