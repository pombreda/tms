set(CMAKE_C_COMPILER "gcc")

if(WIN32)
set(CMAKE_RC_COMPILER "windres") #just a hack
endif(WIN32)

set(CMAKE_CXX_COMPILER "g++")
set(BUILD_SHARED_LIBS ON)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ext/CMakeModules")

cmake_minimum_required(VERSION 2.6) 

include(ExternalProject)

add_definitions(-g -O2 -fsigned-char -freg-struct-return -Wall -Wextra -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code -pedantic)

include_directories(src/common)
project (TMS CXX)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

enable_testing()
add_subdirectory(src bin)

