find_package(Boost COMPONENTS unit_test_framework system chrono REQUIRED)

add_executable(memory_contraption_test 
  memory_contraption_test.cpp
  )

target_link_libraries(memory_contraption_test 
  tms_memory_contraption 
  tms_contraption 
  ${Boost_LIBRARIES}
  )

add_test(memory_contraption_test memory_contraption_test)

add_executable(soci_contraption_test 
  soci_contraption_test.cpp
  )

find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(Log4cplus REQUIRED)
include_directories(SYSTEM 
  ${SOCI_INCLUDE_DIRS} 
  ${SQLITE3_INCLUDE_DIR} 
  ${Boost_INCLUDE_DIRS}
  ${LOG4CPLUS_INCLUDE_DIRS}
  )

target_link_libraries(soci_contraption_test 
  tms_soci_contraption 
  tms_contraption 
  ${Boost_LIBRARIES}
  ${SOCI_sqlite3_PLUGIN} 
  ${SQLITE3_LIBRARIES} 
  ${SOCIDB_LIBRARY}
  )

add_test(soci_contraption_test soci_contraption_test)

add_executable(server_contraption_test 
  server_contraption_test.cpp
  )

target_link_libraries(server_contraption_test 
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

add_test(server_contraption_test server_contraption_test)


