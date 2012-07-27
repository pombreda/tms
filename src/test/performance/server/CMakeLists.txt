find_package(Boost COMPONENTS unit_test_framework system chrono REQUIRED)
find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(Log4cplus REQUIRED)

include_directories(SYSTEM 
  ${SOCI_INCLUDE_DIRS} 
  ${SQLITE3_INCLUDE_DIRS} 
  ${Boost_INCLUDE_DIRS}
  ${LOG4CPLUS_INCLUDE_DIRS}
  )

add_executable(server
  server.cpp
  )

target_link_libraries(server
  tms_soci_contraption 
  tms_server_contraption
  tms_protocol
  tms_contraption 
  tms_model
  ${Boost_LIBRARIES}
  ${SOCI_sqlite3_PLUGIN} 
  ${SQLITE3_LIBRARIES} 
  ${SOCIDB_LIBRARY}
  ${LOG4CPLUS_LIBRARIES}
  )

add_executable(client
  client.cpp
  )

target_link_libraries(client
  tms_soci_contraption 
  tms_server_contraption
  tms_protocol
  tms_contraption 
  tms_model
  ${Boost_LIBRARIES}
  ${SOCI_sqlite3_PLUGIN} 
  ${SQLITE3_LIBRARIES} 
  ${SOCIDB_LIBRARY}
  ${LOG4CPLUS_LIBRARIES}
  )
