
if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif(WIN32)

add_executable(client_app
               main
               client               
	       frames_collection
               dlg_login
	       dlg_user
	       dlg_contact_person
	       dlg_check_column
	       frm_grid
	       frm_grid_incomings
	       frm_grid_companies
	       frm_grid_users	
	       incomings_frame
	       companies_frame
               options_model
	       logger
               options)

find_package(Log4cplus REQUIRED)
include_directories( "${LOG4CPLUS_INCLUDE_DIR}" )

find_package(wxWidgets COMPONENTS base core xrc REQUIRED)
include( "${wxWidgets_USE_FILE}" )

find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
include_directories(SYSTEM ${SOCI_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIR})

target_link_libraries(client_app
  tms_validators
  tms_soci_contraption
  tms_contraption
  tms_contraption_grid
  tms_project
  tms_gui_exception
  tms_model
  tms_server_contraption
  tms_project_model
  ${Boost_LIBRARIES}
  ${SOCI_sqlite3_PLUGIN}
  ${SQLITE3_LIBRARIES}
  ${SOCIDB_LIBRARY}
  ${LOG4CPLUS_LIBRARIES}
  ${wxWidgets_LIBRARIES}
  )

