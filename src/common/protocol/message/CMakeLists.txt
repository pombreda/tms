find_package(ProtocolBuffers REQUIRED)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS PROTOFILES 
  # login
  login_request.proto 
  login_response.proto 
  error_response.proto
  # backend
  record.proto
  filter.proto
  read_records_request.proto
  read_records_response.proto
  write_records_request.proto
  write_records_response.proto
  )

add_library (tms_message SHARED
  login_response
  login_request
  error_response
  record
  read_records_response
  read_records_request
  write_records_response
  write_records_request
  ${PROTO_SRCS}
  ${PROTO_HDRS}
  )
target_link_libraries(tms_message
  ${PROTOBUF_LIBRARY}
)                    
