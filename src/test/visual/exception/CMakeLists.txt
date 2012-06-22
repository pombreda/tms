set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -mwindows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -mwindows")

add_executable (exception_exe main)
set_target_properties (exception_exe PROPERTIES OUTPUT_NAME exception)

set(dlls  ${wxmsw} tms_gui_exception)
target_link_libraries(exception_exe ${dlls} ${win_libs})

foreach(dll ${dlls})
  get_target_property(path ${dll} LOCATION)
  add_custom_command(
   TARGET exception_exe
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${path} ${CMAKE_CURRENT_BINARY_DIR}
   )
endforeach()

foreach(path ${stdlibs})
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

