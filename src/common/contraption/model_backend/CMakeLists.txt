add_library (tms_memory_contraption SHARED
  memory_model_backend.cpp  
  )
target_link_libraries(tms_memory_contraption tms_contraption)

find_package(Soci REQUIRED)

include_directories(${SOCI_INCLUDE_DIRS})

add_library (tms_soci_contraption SHARED
  soci_model_backend.cpp  
  )

target_link_libraries(tms_soci_contraption tms_contraption ${SOCI_LIBRARY})
