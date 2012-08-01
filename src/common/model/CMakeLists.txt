find_package(Boost COMPONENTS signals REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${wxWidget_INCLUDE_DIR})
add_library (tms_model SHARED
  user
  )
target_link_libraries(tms_model tms_contraption tms_exception)