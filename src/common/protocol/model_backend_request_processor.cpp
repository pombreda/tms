#include "model_backend_request_processor.hpp"
// std
#include <cassert>
// log4cplus
#include <log4cplus/loggingmacros.h>
// protobuf
#include <google/protobuf/text_format.h>
// common
#include <protocol/message/login_request.hpp>
#include <protocol/message/record.hpp>
#include <protocol/message/filter.hpp>
#include <protocol/server.hpp>
#include <contraption/field_type.hpp>
#include <contraption/record.hpp>

using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::contraption;
using tms::common::protocol::message::InitRecord;
using tms::common::protocol::message::GetFilter;
using tms::common::protocol::message::ReadRecordsResponse;
using tms::common::protocol::message::ReadRecordsResponseP;
using tms::common::protocol::message::ReadRecordsRequest;
using tms::common::protocol::message::ReadRecordsRequestP;
using tms::common::protocol::message::WriteRecordsResponse;
using tms::common::protocol::message::WriteRecordsResponseP;
using tms::common::protocol::message::WriteRecordsRequest;
using tms::common::protocol::message::WriteRecordsRequestP;
using tms::common::protocol::message::DeleteEntryResponse;
using tms::common::protocol::message::DeleteEntryResponseP;
using tms::common::protocol::message::DeleteEntryRequest;
using tms::common::protocol::message::DeleteEntryRequestP;
using tms::common::protocol::message::SelectResponse;
using tms::common::protocol::message::SelectResponseP;
using tms::common::protocol::message::SelectRequest;
using tms::common::protocol::message::SelectRequestP;

ModelBackendRequestProcessor
::ModelBackendRequestProcessor(Server &server,
                               RequestProcessorP request_processor, 
                               ModelP users,
                               const SOCIDBScheme &scheme) :
    LoginRequestProcessor(server, request_processor, users),
    scheme_(scheme),
    backend_map_() {
}

ModelBackendRequestProcessor
::ModelBackendRequestProcessor(RequestProcessorP request_processor, 
                               ModelP users,
                               const SOCIDBScheme &scheme) :
    LoginRequestProcessor(request_processor, users),
    scheme_(scheme),
    backend_map_() {
}

RequestProcessorP ModelBackendRequestProcessor::Duplicate() const {
  return RequestProcessorP(
      new ModelBackendRequestProcessor(*server_,
                                       request_processor_->Duplicate(), 
                                       users_, 
                                       scheme_));
}

SOCIModelBackendP 
ModelBackendRequestProcessor::GetBackend(const string &table) {
  BackendMap::iterator it = server_->Get<BackendMap>("backend_map").find(table);
  if (it == server_->Get<BackendMap>("backend_map").end()) {
    return (server_->Get<BackendMap>("backend_map"))[table] = SOCIModelBackendP(
        new SOCIModelBackend(scheme_, table));
  }
  return it->second;
}

MessageP ModelBackendRequestProcessor::ReadRecords(
    const ReadRecordsRequest &request) {
  ReadRecordsResponseP response(new ReadRecordsResponse);
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
    GetValue(*records[i], *response->add_record());
  }
  assert(response->record_size() == request.record_size());  
  return response;
}

MessageP ModelBackendRequestProcessor::WriteRecords(
    const WriteRecordsRequest &request) {
  LOG4CPLUS_INFO(logger_, 
                 LOG4CPLUS_TEXT("User " + server_->Get<ContraptionP>("user")
                                ->Get<string>("name")
                                + " writes " + " to " + request.table()));  

  WriteRecordsResponseP response(new WriteRecordsResponse());
  FieldTypeArray values(new boost::scoped_ptr<FieldType>[request.record_size()]);
  vector<RecordP> records(static_cast<size_t>(request.record_size()));
  for (size_t i = 0, end = static_cast<size_t>(request.record_size()); 
       i < end; ++i) {
    InitRecord(request.record(static_cast<int>(i)), 
               values[static_cast<long int>(i)], 
               records[i]);
  }
  SOCIModelBackendP backend = GetBackend(request.table());
  ContraptionID id = static_cast<ContraptionID>(request.id());
  backend->WriteRecords(records, id);
  response->set_id(id);
  return response;
}

MessageP ModelBackendRequestProcessor::DeleteEntry(
    const message::DeleteEntryRequest &request) {
  ContraptionID id = request.id();
  SOCIModelBackendP backend = GetBackend(request.table());
  backend->DeleteEntry(id);
  return DeleteEntryResponseP(new DeleteEntryResponse());
}

MessageP ModelBackendRequestProcessor::Select(
    const message::SelectRequest &request) {  
  string s;
  google::protobuf::TextFormat::PrintToString(request, &s);
  FilterCP filter = GetFilter(request.filter());

  LOG4CPLUS_INFO(logger_, 
                 LOG4CPLUS_TEXT("User " + server_->Get<ContraptionP>("user")
                                ->Get<string>("name")
                                + " selected " + filter->ToString()
                                + " from " + request.table()));  
  SOCIModelBackendP backend = GetBackend(request.table());
  auto_ptr< vector<ContraptionID> > contraptions = backend->Select(filter);
  SelectResponseP response (new SelectResponse());

  for (size_t i = 0, end = contraptions->size(); i < end; ++i) {
    response->add_contraption_id(
        static_cast<google::protobuf::uint32>((*contraptions)[i]));
  }
  return response;
}


MessageP ModelBackendRequestProcessor::Eval(const Message &message, Server &server) {
  if (server_->Check("user")) {
    const ReadRecordsRequest *read_records_request 
        = dynamic_cast<const ReadRecordsRequest*>(&message);
    if (read_records_request) {
      return ReadRecords(*read_records_request);
    }
    const WriteRecordsRequest *write_records_request 
        = dynamic_cast<const WriteRecordsRequest*>(&message);
    if (write_records_request) {
      return WriteRecords(*write_records_request);
    }
    const DeleteEntryRequest *delete_entry_request 
        = dynamic_cast<const DeleteEntryRequest*>(&message);
    if (delete_entry_request) {
      return DeleteEntry(*delete_entry_request);
    }
    const SelectRequest *select_request 
        = dynamic_cast<const SelectRequest*>(&message);
    if (select_request) {
      return Select(*select_request);
    }
  }
  return LoginRequestProcessor::Eval(message, server);
}
