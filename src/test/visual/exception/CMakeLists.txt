if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif(WIN32)

add_executable (exception_exe main)
set_target_properties (exception_exe PROPERTIES OUTPUT_NAME exception)

find_package(wxWidgets COMPONENTS base core xrc REQUIRED) 
include("${wxWidgets_USE_FILE}")
include_directories(${wxWidgets_INCLUDE_DIRS})

target_link_libraries(exception_exe tms_gui_exception ${wxWidgets_LIBRARIES})


