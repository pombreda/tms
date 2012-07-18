add_library (tms_protocol SHARED
  crypto.cpp
  protocol.cpp
  protocol.hpp
  )

find_package(Boost COMPONENTS serialization REQUIRED)

target_link_libraries(tms_protocol tms_exception crypto ${Boost_LIBRARIES})