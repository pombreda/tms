add_library (tms_memory_contraption SHARED
  memory_model_backend
  )
target_link_libraries(tms_memory_contraption tms_contraption)

find_package(Soci REQUIRED)

include_directories(${SOCI_INCLUDE_DIRS})

add_library (tms_soci_contraption SHARED
  soci_model_backend
  )

target_link_libraries(tms_soci_contraption tms_contraption ${SOCI_LIBRARY})

add_library (tms_server_contraption SHARED
  server_model_backend
  )

target_link_libraries(tms_server_contraption 
  tms_contraption 
  tms_protocol
  )
