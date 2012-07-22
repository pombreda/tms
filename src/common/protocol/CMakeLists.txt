add_library (tms_protocol SHARED
  crypto
  protocol
  protocol_impl
  server
  tcp_server
  server_impl
  stream_server
  socket_server
  decorator_server
  client
  message
  )

find_package(Boost COMPONENTS serialization thread REQUIRED)

target_link_libraries(tms_protocol tms_exception crypto ${Boost_LIBRARIES})