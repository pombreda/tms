add_subdirectory(model_backend)

add_library (tms_contraption SHARED
  model.cpp  
  contraption.cpp
  )

#find_package(Boost COMPONENTS serialization REQUIRED)

target_link_libraries(tms_contraption tms_exception)