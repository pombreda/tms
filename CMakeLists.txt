set(CMAKE_C_COMPILER "gcc")

if(WIN32)
set(CMAKE_RC_COMPILER "windres") #just a hack
endif(WIN32)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -O2")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")
set(BUILD_SHARED_LIBS ON)
include(config.txt)

cmake_minimum_required(VERSION 2.6) 

include(ExternalProject)
add_definitions(-g -fsigned-char -freg-struct-return -Wall -m32)
include_directories(src/common ext/include)
project (TMS CXX)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

enable_testing()
add_subdirectory(src bin)

