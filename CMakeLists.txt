set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -O2")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")
include(config.txt)

cmake_minimum_required(VERSION 2.6) 
include(ExternalProject)
add_definitions(-g -fsigned-char -freg-struct-return -Wall -m32)
include_directories(src/common ext/include)
project (TMS CXX)
set(root ${CMAKE_SOURCE_DIR})
string(REPLACE "C:" "/c" nix_root ${root})
set(win_libs rpcrt4 oleaut32 ole32 uuid gdi32 shell32 comctl32 comdlg32 ctl3d32 advapi32 wsock32 winmm winspool)

######################
# stdlibs
######################
find_library(gcc_s_dw2 libgcc_s_dw2-1.dll)
find_library(libstdc++-6 libstdc++-6.dll)
set(stdlibs ${gcc_s_dw2} ${libstdc++-6})

######################
# wxWidgets
######################

set( wxWidgets_Version 2.8 )
string( REPLACE "." "_" wxWidgets_Version_Underscore ${wxWidgets_Version} )

message( STATUS "wxWidgets_Version: " ${wxWidgets_Version} )

if(NOT skip_wxWidget_install)
  externalproject_add(
    wxWidgets-${wxWidgets_Version}
    PREFIX ${root}/ext/src/wxWidgets-${wxWidgets_Version}
    SVN_REPOSITORY http://svn.wxwidgets.org/svn/wx/wxWidgets/branches/WX_${wxWidgets_Version_Underscore}_BRANCH
    CONFIGURE_COMMAND echo "nothing to configure"  
    BUILD_COMMAND echo -e "cd ${nix_root}/ext/src/wxWidgets-${wxWidgets_Version}/src/wxWidgets-${wxWidgets_Version}/build/msw\\nmingw32-make -f makefile.gcc SHARED=1 UNICODE=1 BUILD=release" | sh
    INSTALL_COMMAND echo -e "cp ${nix_root}/ext/src/wxWidgets-${wxWidgets_Version}/src/wxWidgets-${wxWidgets_Version}/lib/gcc_dll/* ${nix_root}/ext/lib -rf\\ncp ${nix_root}/ext/src/wxWidgets-${wxWidgets_Version}/src/wxWidgets-${wxWidgets_Version}/include/* ${nix_root}/ext/include -rf\\n cp ${nix_root}/ext/src/wxWidgets-${wxWidgets_Version}/src/wxWidgets-${wxWidgets_Version}/lib/gcc_dll/mswu/*  ${nix_root}/ext/include -rf\\nsed -i 's/wxUSE_UNICODE 0/wxUSE_UNICODE 1/g' ${nix_root}/ext/include/wx/setup.h" | sh
    )
endif(NOT skip_wxWidget_install)

set(wxmsw wxbase28u wxmsw28u_core wxbase28u_xml wxbase28u_net wxmsw28u_adv wxmsw28u_aui wxmsw28u_html wxmsw28u_media wxmsw28u_richtext wxmsw28u_xrc)

foreach(lib ${wxmsw})
  add_library(${lib} SHARED IMPORTED)
  set_property(TARGET ${lib} PROPERTY 
    IMPORTED_LOCATION ${root}/ext/lib/${lib}_gcc_custom.dll)
  set_property(TARGET ${lib} PROPERTY 
    IMPORTED_IMPLIB ${root}/ext/lib/lib${lib}.a)
endforeach()

######################
# Boost
######################

set(Boost_Version 1.49.0)
string(REPLACE "." "_" Boost_Version_Underscore ${Boost_Version})
set(Boost_Version_Underscore_Short 1_49)

message(STATUS "Boost_Version: " ${Boost_Version})

if(NOT skip_boost_install)
  ExternalProject_Add(
    Boost
    PREFIX ${root}/ext/src/boost${Boost_Version}
    URL 
    http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
    UPDATE_COMMAND echo -e "cd ${root}/ext/src/boost${Boost_Version}/src/Boost\\ncmd /c bootstrap" | sh
    CONFIGURE_COMMAND ""
    BUILD_COMMAND echo -e "cd ${root}/ext/src/boost${Boost_Version}/src/Boost\\n./bjam.exe --without-mpi --prefix=${root}/ext toolset=gcc address-model=32 variant=debug,release link=static,shared threading=multi  install" | sh
    INSTALL_COMMAND echo -e "rm -rf ${nix_root}/ext/include/boost\\nln -s ${nix_root}/ext/include/boost-${Boost_Version_Underscore_Short}/boost ${nix_root}/ext/include" | sh
    )
endif(NOT skip_boost_install)

add_library(boost_unit_test SHARED IMPORTED)
set_property(TARGET boost_unit_test PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libboost_unit_test_framework-mgw46-mt-d-1_49.dll)
set_property(TARGET boost_unit_test PROPERTY 
  IMPORTED_IMPLIB ${root}/ext/lib/libboost_unit_test_framework-mgw46-mt-d-1_49.dll.a)

######################
# OpenSSL
######################

set( OpenSSL_Version 1.0.0j )

message( STATUS "OpenSSL_Version: " ${OpenSSL_Version} )

if(NOT skip_OpenSSL_install)
  ExternalProject_Add(
    OpenSSL
    PREFIX ${root}/ext/src/openssl_${OpenSSL_Version}
    URL 
    http://www.openssl.org/source/openssl-${OpenSSL_Version}.tar.gz
    CONFIGURE_COMMAND echo -e "cd ${root}/ext/src/openssl_${OpenSSL_Version}/src/OpenSSL\\nPERL=perl ./config --prefix=${root}/ext" | sh
    BUILD_COMMAND echo -e "cd ${root}/ext/src/openssl_${OpenSSL_Version}/src/OpenSSL\\nmake" | sh
    INSTALL_COMMAND echo -e "cd ${root}/ext/src/openssl_${OpenSSL_Version}/src/OpenSSL\\nmake install" | sh
    )
endif(NOT skip_OpenSSL_install)  

######################
enable_testing()
add_subdirectory(src bin)

