find_package(Boost COMPONENTS program_options REQUIRED)

find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(Log4cplus REQUIRED)
include_directories(SYSTEM 
  ${SOCI_INCLUDE_DIRS} 
  ${SQLITE3_INCLUDE_DIRS} 
  ${Boost_INCLUDE_DIRS}
  ${LOG4CPLUS_INCLUDE_DIRS}
  )

add_executable(server_app
  server
  main
  )

target_link_libraries(server_app
  tms_protocol
  tms_contraption
  tms_model
  tms_project_model
  tms_project
  ${SOCI_sqlite3_PLUGIN} 
  ${SQLITE3_LIBRARIES} 
  ${SOCIDB_LIBRARY}
  ${Boost_LIBRARIES}
)
