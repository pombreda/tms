find_package(wxWidgets COMPONENTS base core xrc REQUIRED) 
include("${wxWidgets_USE_FILE}")
include_directories(${wxWidgets_INCLUDE_DIRS})
add_library(tms_gui_exception SHARED gui_exception_report_dialog.cpp gui_exception.cpp)
add_library(tms_exception SHARED gui_exception.cpp)                             

target_link_libraries(tms_gui_exception ${wxWidgets_LIBRARIES})
