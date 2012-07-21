add_library (tms_protocol SHARED
  crypto
  protocol
  protocol_impl
  server
  server_impl
  client
  message
  )

find_package(Boost COMPONENTS serialization thread REQUIRED)

target_link_libraries(tms_protocol tms_exception crypto ${Boost_LIBRARIES})