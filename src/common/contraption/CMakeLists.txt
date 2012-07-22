add_subdirectory(model_backend)

add_library (tms_contraption SHARED
  model
  contraption
  contraption_array
  )

find_package(Boost COMPONENTS signals REQUIRED)

target_link_libraries(tms_contraption tms_exception ${Boost_LIBRARIES})