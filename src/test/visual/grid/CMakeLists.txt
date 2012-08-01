find_package(Boost COMPONENTS signals REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${wxWidget_INCLUDE_DIR})
if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif(WIN32)

add_executable (grid_exe main)
set_target_properties (grid_exe PROPERTIES OUTPUT_NAME grid)

find_package(wxWidgets COMPONENTS base core xrc REQUIRED)
include( "${wxWidgets_USE_FILE}" )

find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
include_directories(${SOCI_INCLUDE_DIRS})

target_link_libraries(grid_exe
  tms_soci_contraption
  tms_contraption
  tms_contraption_grid
  ${SOCI_sqlite3_PLUGIN}
  ${SQLITE3_LIBRARIES}
  ${SOCIDB_LIBRARY}
  ${wxWidgets_LIBRARIES}
  )
