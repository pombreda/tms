add_library(tms_contraption_grid SHARED
            contraption_grid
            printer)

find_package(wxWidgets COMPONENTS base core xrc REQUIRED)
include( "${wxWidgets_USE_FILE}" )
find_package(Boost COMPONENTS signals REQUIRED)

target_link_libraries(tms_contraption_grid
  tms_contraption
  ${wxWidgets_LIBRARIES}
  )
