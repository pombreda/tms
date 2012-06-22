set (exception_src  )
add_library (tms_gui_exception SHARED gui_exception_report_dialog.cpp gui_exception.cpp)
add_library (tms_exception SHARED gui_exception.cpp)                             
message("${wxms}")

add_dependencies(tms_gui_exception wxWidgets-2.8)
target_link_libraries(tms_gui_exception ${wxmsw})

