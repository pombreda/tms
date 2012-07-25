if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0501")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0501")
endif(WIN32)

add_subdirectory(message)
add_library (tms_protocol SHARED
  crypto
  protocol
  protocol_impl
  protobuf_clean
  server
  tcp_server
  server_impl
  stream_server
  socket_server
  request_processor
  simple_request_processor
  login_request_processor  
  model_backend_request_processor
  login_protocol
  model_backend_protocol
  client
  message
  )

find_package(Boost COMPONENTS serialization thread system chrono REQUIRED)
target_link_libraries(tms_protocol 
  tms_exception 
  tms_message 
  tms_contraption 
  tms_soci_contraption
  crypto 
  ${Boost_LIBRARIES})
if(WIN32)
  target_link_libraries(tms_protocol ws2_32 mswsock)
endif(WIN32)


