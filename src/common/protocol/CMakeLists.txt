add_library (tms_protocol SHARED
  crypto.cpp
  )

find_package(Boost COMPONENTS serialization REQUIRED)

target_link_libraries(tms_protocol tms_exception crypto ${Boost_LIBRARIES})