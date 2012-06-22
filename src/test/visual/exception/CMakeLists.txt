set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -mwindows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -mwindows")

add_executable (exception_exe main)
set_target_properties (exception_exe PROPERTIES OUTPUT_NAME exception)

add_dependencies(exception_exe wxWidgets-2.8)
set(dlls  ${wxmsw} tms_gui_exception)
target_link_libraries(exception_exe ${dlls})

foreach(dll ${dlls})
  get_target_property(path ${dll} LOCATION)
  add_custom_command(
   TARGET exception_exe
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${path} ${CMAKE_CURRENT_BINARY_DIR}
   )
endforeach()

add_custom_command(
  TARGET exception_exe
  POST_BUILD
  COMMAND mkdir
  ARGS ${CMAKE_CURRENT_BINARY_DIR}/xrc -p
  )

add_custom_command(
  TARGET exception_exe
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${root}/xrc/common/gui_exception/gui_exception_report_dialog.xrc ${CMAKE_CURRENT_BINARY_DIR}/xrc
  )

add_custom_command(
  TARGET exception_exe
  POST_BUILD
  COMMAND mkdir
  ARGS ${CMAKE_CURRENT_BINARY_DIR}/img -p
  )

add_custom_command(
  TARGET exception_exe
  POST_BUILD
  COMMAND mkdir
  ARGS ${CMAKE_CURRENT_BINARY_DIR}/img/icons -p
  )

add_custom_command(
  TARGET exception_exe
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${root}/img/icons/messagebox_warning.png  ${CMAKE_CURRENT_BINARY_DIR}/img/icons/messagebox_warning.png 
  )

if(WIN32)
  target_link_libraries (exception_exe rpcrt4 oleaut32 ole32 uuid gdi32 shell32 comctl32 comdlg32 ctl3d32 advapi32 wsock32 winmm winspool)
endif(WIN32)

if(UNIX)
  target_link_libraries (exception_exe Xext X11 fontconfig Xft Xinerama)
endif(UNIX)
