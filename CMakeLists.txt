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
set(root ${CMAKE_SOURCE_DIR})
string(REPLACE "C:" "/c" nix_root ${root})

######################
# wxWidgets
######################

externalproject_add(
  wxWidgets-2.8
  PREFIX ${root}/ext/src/wxWidgets-2.8
  SVN_REPOSITORY http://svn.wxwidgets.org/svn/wx/wxWidgets/branches/WX_2_8_BRANCH
  CONFIGURE_COMMAND echo "nothing to configure"  
  BUILD_COMMAND echo -e "cd ${nix_root}/ext/src/wxWidgets-2.8/src/wxWidgets-2.8/build/msw\\nmingw32-make -f makefile.gcc SHARED=1 UNICODE=1 BUILD=release" | sh
  INSTALL_COMMAND echo -e "cp ${nix_root}/ext/src/wxWidgets-2.8/src/wxWidgets-2.8/lib/gcc_dll/* ${nix_root}/ext/lib -rf\\ncp ${nix_root}/ext/src/wxWidgets-2.8/src/wxWidgets-2.8/include/* ${nix_root}/ext/include -rf\\n cp ${nix_root}/ext/src/wxWidgets-2.8/src/wxWidgets-2.8/lib/gcc_dll/mswu/*  ${nix_root}/ext/include -rf\\nsed -i 's/wxUSE_UNICODE 0/wxUSE_UNICODE 1/g' ${nix_root}/ext/include/wx/setup.h" | sh
)

set(wxmsw wxbase28u wxmsw28u_core wxbase28u_xml wxbase28u_net wxmsw28u_adv wxmsw28u_aui wxmsw28u_html wxmsw28u_media wxmsw28u_richtext wxmsw28u_xrc)

foreach(lib ${wxmsw})
  add_library(${lib} SHARED IMPORTED)
  set_property(TARGET ${lib} PROPERTY 
    IMPORTED_LOCATION ${root}/ext/lib/${lib}_gcc_custom.dll)
  set_property(TARGET ${lib} PROPERTY 
    IMPORTED_IMPLIB ${root}/ext/lib/lib${lib}.a)
endforeach()

######################

add_subdirectory(src bin)