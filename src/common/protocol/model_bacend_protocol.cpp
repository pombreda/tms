#include "login_protocol.hpp"
#include <protocol/message/read_records_request.hpp>
#include <protocol/message/read_records_response.hpp>
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;

ModelBackendProtocol::ModelBackendProtocol() 
    throw(ProtocolException): 
    Protocol() {
  AddClasses();
}

ModelBackendProtocol::ModelBackendProtocol(const Protocol &other) 
    throw(ProtocolException): 
    Protocol(other) {
  AddClasses();
}

void ModelBackendProtocol::AddClasses() 
    throw(ProtocolException) {
  AddMessageClass<ReadRecordsResponse>();
  AddMessageClass<ReadRecordsRequest>();
}
