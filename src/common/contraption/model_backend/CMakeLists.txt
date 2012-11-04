find_package(Boost COMPONENTS signals REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_library (tms_memory_contraption SHARED
  memory_model_backend
  )
target_link_libraries(tms_memory_contraption tms_contraption)

find_package(Soci REQUIRED)
find_package(Log4cplus REQUIRED)
include_directories(SYSTEM ${SOCI_INCLUDE_DIRS} ${LOG4CPLUS_INCLUDE_DIR})


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
