add_library (tms_memory_contraption SHARED
  memory_model_backend.cpp  
  )
target_link_libraries(tms_memory_contraption tms_contraption)