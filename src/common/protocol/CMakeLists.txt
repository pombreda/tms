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
  patcher_request_processor
  login_protocol
  model_backend_protocol
  patcher_protocol
  client
  message
  socket
  socket_client
  )

find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS serialization thread system chrono filesystem REQUIRED)
find_package(Log4cplus REQUIRED)
find_package(Soci REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${LOG4CPLUS_INCLUDE_DIR} ${OpenSSL_INCLUDE_DIR} ${SOCI_INCLUDE_DIRS})
target_link_libraries(tms_protocol 
  tms_string
  tms_exception 
  tms_message 
  tms_patcher
  tms_contraption 
  tms_soci_contraption
  crypto 
  ${Boost_LIBRARIES}
  ${LOG4CPLUS_LIBRARIES}
  ${OpenSSL_LIBRARIES}
)

if(WIN32)
  target_link_libraries(tms_protocol ws2_32 mswsock)
endif(WIN32)


