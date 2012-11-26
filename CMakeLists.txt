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

add_definitions(-g -O2 -fsigned-char -freg-struct-return -Wall -Wextra -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Winit-self -Wunreachable-code -pedantic -DBOOST_THREAD_USE_LIB -DUNICODE -D_UNICODE)

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

if (${CMAKE_SOURCE_DIR}/src/config.hpp.in IS_NEWER_THAN ${CMAKE_SOURCE_DIR}/bin/config.hpp)
  configure_file (
    ${CMAKE_SOURCE_DIR}/src/config.hpp.in
    ${CMAKE_SOURCE_DIR}/bin/config.hpp)
endif(${CMAKE_SOURCE_DIR}/src/config.hpp.in IS_NEWER_THAN ${CMAKE_SOURCE_DIR}/bin/config.hpp)

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
  file(GLOB libs "${CMAKE_SOURCE_DIR}/lib/*.so")
  file(GLOB client "${CMAKE_SOURCE_DIR}/bin/client/client*")
  file(GLOB server "${CMAKE_SOURCE_DIR}/bin/server/server*")
elseif(WIN32)
  file(GLOB libs "${CMAKE_SOURCE_DIR}/lib/*.dll")
  file(GLOB client "${CMAKE_SOURCE_DIR}/bin/client/*.exe")
  file(GLOB server "${CMAKE_SOURCE_DIR}/bin/server/*.exe")
endif(UNIX)

if(WIN32)
  foreach(lib
          "libgcc_s_dw2-1.dll"
          "libstdc++-6.dll"
          "libboost_signals-mgw47-mt-1_50.dll"
          "libboost_system-mgw47-mt-1_50.dll"
          "libboost_thread-mgw47-mt-1_50.dll"
          "libboost_chrono-mgw47-mt-1_50.dll"
          "libboost_filesystem-mgw47-mt-1_50.dll"
          "libboost_program_options-mgw47-mt-1_50.dll"
          "libboost_signals-mgw46-mt-1_50.dll"
          "libboost_system-mgw46-mt-1_50.dll"
          "libboost_thread-mgw46-mt-1_50.dll"
          "libboost_chrono-mgw46-mt-1_50.dll"
          "libboost_filesystem-mgw46-mt-1_50.dll"
          "libboost_program_options-mgw46-mt-1_50.dll"
          "libboost_signals-mgw45-mt-1_50.dll"
          "libboost_system-mgw45-mt-1_50.dll"
          "libboost_thread-mgw45-mt-1_50.dll"
          "libboost_chrono-mgw45-mt-1_50.dll"
          "libboost_filesystem-mgw45-mt-1_50.dll"
          "libboost_program_options-mgw45-mt-1_50.dll"
          "wxbase294u_gcc_custom.dll"
          "wxbase294u_xml_gcc_custom.dll"
          "wxmsw294u_adv_gcc_custom.dll"
          "wxmsw294u_core_gcc_custom.dll"
          "wxmsw294u_xrc_gcc_custom.dll"
          "wxmsw294u_html_gcc_custom.dll"
          "libprotobuf-7.dll"
          "libsoci_core_3_1.dll"
          "libsoci_sqlite3_3_1.dll"
          "libsqlite3-0.dll"
          "liblog4cplus-1-0-4.dll"
          "libarchive-12.dll"
          )
    message("${lib}")
    unset(lib_path CACHE)
    find_file(
      lib_path
      ${lib}
      PATHS ENV PATH
    )
    if(lib_path)
      message("${lib_path}")
      list(APPEND system_libs ${lib_path})
    endif(lib_path)
  endforeach(lib)
  unset(lib_path CACHE)
endif(WIN32)

# server
install(FILES
  ${server}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/server)

install(FILES
  "log.cfg.default"
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/server RENAME log.cfg)

install(FILES
  ${libs}
  PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/server)

if(WIN32)
  install(FILES
    ${system_libs}
    PERMISSIONS OWNER_READ OWNER_WRITE
    DESTINATION ${CMAKE_SOURCE_DIR}/release/server)
endif(WIN32)

# client
install(FILES
  ${client}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

install(FILES
  "log.cfg.default"
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client RENAME log.cfg)

install(FILES
  ${libs}
  PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

if(WIN32)
  install(FILES
    ${system_libs}
    PERMISSIONS OWNER_READ OWNER_WRITE
    DESTINATION ${CMAKE_SOURCE_DIR}/release/client)
endif(WIN32)

install(DIRECTORY
  ${CMAKE_SOURCE_DIR}/xrc
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

install(DIRECTORY
  ${CMAKE_SOURCE_DIR}/img
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE
  DESTINATION ${CMAKE_SOURCE_DIR}/release/client)

