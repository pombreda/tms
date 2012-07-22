add_library (tms_model SHARED
  user
  )
target_link_libraries(tms_model tms_contraption tms_exception)