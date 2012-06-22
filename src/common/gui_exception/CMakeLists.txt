set (exception_src gui_exception.cpp gui_exception_report_dialog.cpp)
add_library (tms_gui_exception SHARED ${exception_src})
message("${wxms}")

add_dependencies(tms_gui_exception wxWidgets-2.8)
target_link_libraries(tms_gui_exception ${wxmsw})

