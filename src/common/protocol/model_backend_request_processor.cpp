#include "model_backend_request_processor.hpp"
// std
#include <cassert>
// common
#include <protocol/message/login_request.hpp>
#include <protocol/message/record.hpp>
#include <contraption/field_type.hpp>
#include <contraption/record.hpp>

using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::contraption;
using tms::common::protocol::message::InitRecord;
using tms::common::protocol::message::ReadRecordsResponse;
using tms::common::protocol::message::ReadRecordsResponseP;
using tms::common::protocol::message::ReadRecordsRequest;
using tms::common::protocol::message::ReadRecordsRequestP;

ModelBackendRequestProcessor
::ModelBackendRequestProcessor(RequestProcessorP request_processor, 
                               ModelP users,
                               const SOCIDBScheme &scheme) :
    LoginRequestProcessor(request_processor, users),
    scheme_(scheme),
    backend_map_() {
}

RequestProcessorP ModelBackendRequestProcessor::Duplicate() {
  return RequestProcessorP(
      new ModelBackendRequestProcessor(request_processor_->Duplicate(), 
                                       users_, scheme_));
}

SOCIModelBackendP 
ModelBackendRequestProcessor::GetBackend(const string &table) {
  BackendMap::iterator it = backend_map_.find(table);
  if (it == backend_map_.end()) {
    return backend_map_[table] = SOCIModelBackendP(
        new SOCIModelBackend(scheme_, table));
  }
  return it->second;
}

ReadRecordsResponseP ModelBackendRequestProcessor::ReadRecords(
    const ReadRecordsRequest &request) {
  ReadRecordsResponseP response;
  FieldTypeArray values(new boost::scoped_ptr<FieldType>[request.record_size()]);
  vector<RecordP> records(static_cast<size_t>(request.record_size()));
  for (size_t i = 0, end = static_cast<size_t>(request.record_size()); 
       i < end; ++i) {
    InitRecord(request.record(static_cast<int>(i)), 
               values[static_cast<long int>(i)], 
               records[i]);
  }
  SOCIModelBackendP backend = GetBackend(request.table());
  backend->ReadRecords(records, request.id());
  for (size_t i = 0, end = static_cast<size_t>(request.record_size()); 
       i < end; ++i) {
    GetValue(*records[i], *response->add_record());;
  }
  assert(response->record_size() == request.record_size());  
  return response;
}

MessageP ModelBackendRequestProcessor::Eval(const Message &message) {
  if (user_) {
    const ReadRecordsRequest *read_records_request 
        = dynamic_cast<const ReadRecordsRequest*>(&message);
    if (read_records_request) {
      return ReadRecords(*read_records_request);
    }
  }
  return LoginRequestProcessor::Eval(message);
}
