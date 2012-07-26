#include "model_backend_protocol.hpp"
// common
#include <protocol/message/read_records_request.hpp>
#include <protocol/message/read_records_response.hpp>
#include <protocol/message/write_records_request.hpp>
#include <protocol/message/write_records_response.hpp>
#include <protocol/message/delete_entry_request.hpp>
#include <protocol/message/delete_entry_response.hpp>
#include <protocol/message/select_response.hpp>
#include <protocol/message/select_request.hpp>
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;

ModelBackendProtocol::ModelBackendProtocol() 
    throw(ProtocolException): 
    LoginProtocol() {
  AddClasses();
}

ModelBackendProtocol::ModelBackendProtocol(const Protocol &other) 
    throw(ProtocolException): 
    LoginProtocol(other) {
  AddClasses();
}

void ModelBackendProtocol::AddClasses() 
    throw(ProtocolException) {
  AddMessageClass<ReadRecordsResponse>();
  AddMessageClass<ReadRecordsRequest>();
  AddMessageClass<WriteRecordsResponse>();
  AddMessageClass<WriteRecordsRequest>();
  AddMessageClass<DeleteEntryResponse>();
  AddMessageClass<DeleteEntryRequest>();
  AddMessageClass<SelectResponse>();
  AddMessageClass<SelectRequest>();
}
