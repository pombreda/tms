add_subdirectory(model_backend)

add_library (tms_contraption SHARED
  model
  contraption
  contraption_array
  field
  field_exception
  model_backend
  model_backend_exception
  contraption_fwd
  field_fwd
  field_type
  field_type_fwd
  filter
  )

find_package(Boost COMPONENTS signals REQUIRED)

target_link_libraries(tms_contraption tms_exception ${Boost_LIBRARIES})