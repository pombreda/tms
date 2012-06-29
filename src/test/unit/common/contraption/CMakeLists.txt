find_package(Boost COMPONENTS unit_test_framework REQUIRED)

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

target_link_libraries(soci_contraption_test 
  tms_soci_contraption 
  tms_contraption 
  ${Boost_LIBRARIES}
  ${SOCI_sqlite3_PLUGIN} 
  ${SQLITE3_LIBRARIES} 
  ${SOCIDB_LIBRARY}
  )

message(${SOCI_sqlite3_PLUGIN} ${SQLITE3_LIBRARIES})

add_test(soci_contraption_test soci_contraption_test)

