
find_package(wxWidgets COMPONENTS base core xrc REQUIRED) 
include("${wxWidgets_USE_FILE}")
message("test ${wxWidgets_INCLUDE_DIRS}")
include_directories(${wxWidgets_INCLUDE_DIRS})
add_library(tms_gui_exception SHARED gui_exception_report_dialog.cpp gui_exception.cpp)
add_library(tms_exception SHARED gui_exception.cpp)                             
if(WIN32)
  SET_TARGET_PROPERTIES(tms_gui_exception PROPERTIES
    CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows"
    COMPILE_FLAGS "-DC_BUILDING_LIBCHICKEN ${SHARED_FLAGS}"
    CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows"
  )
endif(WIN32)

target_link_libraries(tms_gui_exception ${wxWidgets_LIBRARIES})
