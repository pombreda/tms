add_executable (exception_exe main)
set_target_properties (exception_exe PROPERTIES OUTPUT_NAME exception)

set(root ${CMAKE_SOURCE_DIR})

add_library(fltk STATIC IMPORTED)
set_property(TARGET fltk PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk.a )

add_library(fltk_forms UNKNOWN IMPORTED)
set_property(TARGET fltk_forms PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk_forms.a )

add_library(fltk_images UNKNOWN IMPORTED)
set_property(TARGET fltk_images PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk_images.a )

add_library(fltk_gl UNKNOWN IMPORTED)
set_property(TARGET fltk_gl PROPERTY 
  IMPORTED_LOCATION ${root}/ext/lib/libfltk_gl.a )

add_dependencies(fltk EP_fltk-1.3)
add_dependencies(fltk_forms EP_fltk-1.3)
add_dependencies(fltk_images EP_fltk-1.3)
add_dependencies(fltk_gl EP_fltk-1.3)


target_link_libraries (exception_exe tms_exception fltk_forms fltk_images fltk_gl fltk)
if (WIN32)
 target_link_libraries (exception_exe rpcrt4 oleaut32 ole32 uuid gdi32 shell32 comctl32 comdlg32 ctl3d32 advapi32 wsock32 winmm winspool)
endif(WIN32)
if (UNIX)
  target_link_libraries (exception_exe Xext X11 fontconfig Xft Xinerama)
endif(UNIX)
