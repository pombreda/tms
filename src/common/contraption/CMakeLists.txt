add_subdirectory(model_backend)

find_package(Boost COMPONENTS signals REQUIRED)
find_package(Log4cplus)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${LOG4CPLUS_INCLUDE_DIRS})

add_library (tms_contraption SHARED
  model
  contraption
  contraption_array
  model_contraption_array
  has_many_field_contraption_array
  field
  field/simple_field
  field/has_many_field
  field/date_field
  field/simple_field_fwd
  field_exception
  model_backend
  model_backend_exception
  contraption_fwd
  compare
  model_wrapper
  record
  record_fwd
  contraption_accessor
  contraption_accessor_fwd
  contraption_impl
  
  field_fwd
  field_type
  field_type_fwd
  filter
  )

target_link_libraries(tms_contraption tms_exception ${Boost_LIBRARIES})
