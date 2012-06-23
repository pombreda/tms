set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -mwindows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -mwindows")

add_executable (exception_exe main)
set_target_properties (exception_exe PROPERTIES OUTPUT_NAME exception)

find_package(wxWidgets COMPONENTS base core xrc REQUIRED) 
include( "${wxWidgets_USE_FILE}" )

target_link_libraries(exception_exe tms_gui_exception ${wxWidgets_LIBRARIES})


