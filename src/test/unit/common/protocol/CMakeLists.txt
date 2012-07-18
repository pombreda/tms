find_package(Boost COMPONENTS unit_test_framework serialization signals REQUIRED)
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
add_test(protocol_test protocol_test)