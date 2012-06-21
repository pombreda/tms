set (exception_src guiexception.cpp)
add_library (tms_exception STATIC ${exception_src})
add_dependencies(tms_exception fltk-1.3)

