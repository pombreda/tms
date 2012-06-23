add_library (tms_protocol SHARED
  request.cpp
  crypto.cpp
  )

target_link_libraries(tms_protocol tms_exception crypto)