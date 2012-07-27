if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif(WIN32)

add_executable(client_app
               main
               login_frame
               grid_frame
               test_frame
               options_model
               options)

find_package(wxWidgets COMPONENTS base core xrc REQUIRED)
include( "${wxWidgets_USE_FILE}" )

find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
include_directories(${SOCI_INCLUDE_DIRS})

target_link_libraries(client_app
  tms_soci_contraption
  tms_contraption
  tms_contraption_grid
  ${Boost_LIBRARIES}
  ${SOCI_sqlite3_PLUGIN}
  ${SQLITE3_LIBRARIES}
  ${SOCIDB_LIBRARY}
  ${wxWidgets_LIBRARIES}
  )

