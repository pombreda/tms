add_subdirectory(message)
add_library (tms_protocol SHARED
  crypto
  protocol
  protocol_impl
  server
  tcp_server
  server_impl
  stream_server
  socket_server
  request_processor
  simple_request_processor
  login_request_processor
  client
  message
  )

find_package(Boost COMPONENTS serialization thread REQUIRED)
target_link_libraries(tms_protocol tms_exception tms_message tms_contraption crypto ${Boost_LIBRARIES})