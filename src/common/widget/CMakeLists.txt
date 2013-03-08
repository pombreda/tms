add_subdirectory(validators)
add_library(tms_contraption_grid SHARED
            contraption_grid
            contraption_grid_table_base
	    contraption_dialog
            printer)

find_package(wxWidgets COMPONENTS base core xrc adv REQUIRED)
include( "${wxWidgets_USE_FILE}" )
find_package(Boost COMPONENTS signals REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${wxWidget_INCLUDE_DIR})
find_package(Log4cplus REQUIRED)
include_directories( "${LOG4CPLUS_INCLUDE_DIR}" )


target_link_libraries(tms_contraption_grid
  tms_contraption
  tms_string
  ${wxWidgets_LIBRARIES}
  ${LOG4CPLUS_LIBRARIES}
  )
