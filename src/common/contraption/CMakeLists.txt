add_subdirectory(model_backend)

add_library (tms_contraption SHARED
  model
  contraption
  contraption_array
  model_contraption_array
  has_many_field_contraption_array
  field
  field/simple_field
  field/has_many_field
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