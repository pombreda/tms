find_package(Boost COMPONENTS unit_test_framework serialization signals system REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
find_package(ProtocolBuffers REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS PROTOFILES dummy.proto)

add_executable(protocol_test 
  protocol_test.cpp
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)
target_link_libraries(protocol_test 
  tms_protocol 
  ${Boost_LIBRARIES} 
  ${PROTOBUF_LIBRARY}
)
if(WIN32)
  target_link_libraries(protocol_test ws2_32 mswsock)
endif(WIN32)

add_test(protocol_test protocol_test)

add_executable(server_client_test 
  server_client_test.cpp
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)
target_link_libraries(server_client_test 
  tms_protocol 
  ${Boost_LIBRARIES} 
  ${PROTOBUF_LIBRARY}
)

if(WIN32)
  target_link_libraries(server_client_test ws2_32 mswsock)
endif(WIN32)

add_test(server_client_test server_client_test)

find_package(Soci REQUIRED)
find_package(Sqlite3 REQUIRED)
include_directories(${SOCI_INCLUDE_DIRS})

add_executable(login_server_test 
  login_server_test.cpp
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)
target_link_libraries(login_server_test 
  tms_protocol 
  tms_contraption
  tms_soci_contraption
  tms_model
  ${Boost_LIBRARIES} 
  ${SOCI_sqlite3_PLUGIN} 
  ${SQLITE3_LIBRARIES} 
  ${SOCIDB_LIBRARY}
  ${PROTOBUF_LIBRARY}
)

if(WIN32)
  target_link_libraries(login_server_test ws2_32 mswsock)
endif(WIN32)

add_test(login_server_test login_server_test)

