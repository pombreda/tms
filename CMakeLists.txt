set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")

cmake_minimum_required(VERSION 2.6) 
include(ExternalProject)
add_definitions(-g -fsigned-char -freg-struct-return -Wall -m32)
include_directories(src/common ext/include)
project (TMS CXX)
add_subdirectory(src bin)
set(root ${CMAKE_SOURCE_DIR})
string(REPLACE "C:" "/c" nix_root ${root})
message("root = ${root}")

#wxWidgets
externalproject_add(
  wxWidgets-2.8
  PREFIX ${root}/ext/src/wxWidgets-2.8
  SVN_REPOSITORY http://svn.wxwidgets.org/svn/wx/wxWidgets/branches/WX_2_8_BRANCH
  CONFIGURE_COMMAND echo "nothing to configure"  
  BUILD_COMMAND echo -e "cd ${nix_root}/ext/src/wxWidgets-2.8/src/wxWidgets-2.8/build/msw\\nmingw32-make -f makefile.gcc SHARED=1 UNICODE=1 BUILD=release" | sh
  INSTALL_COMMAND echo "installing"
)

# FLTK-1.3
externalproject_add(
  EP_fltk-1.3
  PREFIX ${root}/ext/src/fltk-1.3
  SVN_REPOSITORY http://svn.easysw.com/public/fltk/fltk/branches/branch-1.3
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${root}/ext
)

add_library(fltk STATIC IMPORTED)
set_property(TARGET fltk PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk.a )
add_dependencies(fltk EP_fltk-1.3)

add_library(fltk_forms UNKNOWN IMPORTED)
set_property(TARGET fltk_forms PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk_forms.a )
add_dependencies(fltk_forms EP_fltk-1.3)

add_library(fltk_gl UNKNOWN IMPORTED)
set_property(TARGET fltk_gl PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk_gl.a )
add_dependencies(fltk_gl EP_fltk-1.3)

add_library(fltk_images UNKNOWN IMPORTED)
set_property(TARGET fltk_images PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk_images.a )
add_dependencies(fltk_images EP_fltk-1.3)

