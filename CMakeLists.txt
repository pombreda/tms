set(CMAKE_C_COMPILER "gcc")

if(WIN32)
set(CMAKE_RC_COMPILER "windres") #just a hack
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
endif(WIN32)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project (TMS CXX C)

set(BUILD_SHARED_LIBS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ext/CMakeModules")

cmake_minimum_required(VERSION 2.6) 

include(ExternalProject)

add_definitions(-g -O2 -fsigned-char -freg-struct-return -Wall -Wextra -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Winit-self -Wunreachable-code -pedantic -DBOOST_THREAD_USE_LIB)

if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501) # for asio
endif(WIN32)

include(CheckTypeSize)
check_type_size("void*" SIZEOF_VOID_P BUILTIN_TYPES_ONLY)

set(bitness 32)
if(SIZEOF_VOID_P EQUAL 8)
  set(bitness 64) 
endif()

if(WIN32)
  if (bitness EQUAL 32)
    set(system "windows32")
  else(bitness EQUAL 32)
    set(system "windows64")
  endif(bitness EQUAL 32)
elseif(UNIX)
  if (bitness EQUAL 32)
    set(system "linux32")
  else(bitness EQUAL 32)
    set(system "linux64")
  endif(bitness EQUAL 32)
endif(WIN32)

if(NOT system)
  message(FATAL_ERROR "unknown system")
else(NOT system)
  message(STATUS "Target system: ${system}")
endif(NOT system)

string(TOUPPER "${system}" SYSTEM)

configure_file (
  "${CMAKE_SOURCE_DIR}/src/config.hpp.in" 
  "${CMAKE_SOURCE_DIR}/bin/config.hpp")

include_directories(src/common)
include_directories(src)
include_directories(SYSTEM bin/common)
include_directories(SYSTEM bin)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/release)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/release)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/release/server)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/release/client)

enable_testing()
add_subdirectory(src bin)


if(UNIX)
FILE(GLOB libs "${CMAKE_SOURCE_DIR}/lib/*.so")
FILE(GLOB client "${CMAKE_SOURCE_DIR}/bin/client/client_app")
FILE(GLOB server "${CMAKE_SOURCE_DIR}/bin/server/server_app")
elseif(WIN32)
FILE(GLOB libs "${CMAKE_SOURCE_DIR}/lib/*.dll")
FILE(GLOB client "${CMAKE_SOURCE_DIR}/bin/client/client_app.exe")
FILE(GLOB server "${CMAKE_SOURCE_DIR}/bin/client/server_app.exe")
endif(UNIX)

# server
install(FILES 
  ${server}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/server)

install(FILES 
  ${libs} 
  PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/server)

# client
install(FILES 
  ${client} 
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

install(FILES 
  ${libs} 
  PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

install(DIRECTORY
  ${CMAKE_SOURCE_DIR}/xrc
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

install(DIRECTORY
  ${CMAKE_SOURCE_DIR}/img
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

