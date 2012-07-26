#include "server_model_backend.hpp"
// boost
#include <boost/foreach.hpp>
// common
#include <protocol/message/record.hpp>
#include <protocol/message/read_records_request.hpp>
#include <protocol/message/read_records_response.hpp>
#include <protocol/message/write_records_request.hpp>
#include <protocol/message/write_records_response.hpp>
#include <protocol/message/delete_entry_request.hpp>
#include <protocol/message/delete_entry_response.hpp>
#include <protocol/message/filter.hpp>
#include <protocol/message/select_request.hpp>
#include <protocol/message/select_response.hpp>
#include <contraption/contraption.hpp>
#include <contraption/record.hpp>

using namespace tms::common::contraption;
using namespace tms::common::protocol;
using tms::common::protocol::message::GetFreeRecord;
using tms::common::protocol::message::GetValue;
using tms::common::protocol::message::GetFilter;

ServerModelBackend::ServerModelBackend(const protocol::ClientP client, 
                                       const std::string &table_name) 
    throw(ModelBackendException) :
    client_(client),
    table_name_(table_name) {
}

void ServerModelBackend::ReadRecords(
    const std::vector<RecordP> &records,
    ContraptionID id)
    throw(ModelBackendException) {
  if (id == Contraption::kNewID) {
    throw ModelBackendException("Trying to read from "
                                "ContraptionID::kNewID " 
                                "in ServerModelBackend::ReadRecords.");
  }  
  message::ReadRecordsRequestP request(new message::ReadRecordsRequest());
  request->set_table(table_name_);
  request->set_id(static_cast<google::protobuf::uint32>(id));
  BOOST_FOREACH(RecordP record, records) {
    message::Record *new_record = request->add_record();
    GetFreeRecord(*record, *new_record);
  }
  try {
    message::ReadRecordsResponseP response
        = boost::dynamic_pointer_cast<message::ReadRecordsResponse>(
            client_->EvalRequest(*request));
    if (!response) {
      throw ModelBackendException("Unsupported response from client in "
                                  "ServerModelBackend::ReadRecords.");
    }
    if (static_cast<size_t>(response->record_size()) != records.size()) {
      ostringstream msg;
      msg << "Response contain ammount of records "
          << "different from ammount requested: "
          << response->record_size()  << " != " 
          << records.size() << ".";
      throw ModelBackendException(msg.str());    
    }
    for (size_t i = 0, end = records.size(); i < end; ++i) {
      GetValue(response->record(static_cast<int>(i)), *records[i]);
    }
  } catch (ProtocolException &e) {
    throw ModelBackendException(&e);
  }
}

void ServerModelBackend::WriteRecords(
    const std::vector<RecordP> &records,
    ContraptionID &id)
    throw(ModelBackendException) {
  if (records.empty()) {
    return;
  }

  message::WriteRecordsRequestP request(new message::WriteRecordsRequest());
  request->set_table(table_name_);
  request->set_id(static_cast<google::protobuf::uint32>(id));
  BOOST_FOREACH(RecordP record, records) {
    message::Record *new_record = request->add_record();
    GetRecord(*record, *new_record);
  }
  try {
    message::WriteRecordsResponseP response
        = boost::dynamic_pointer_cast<message::WriteRecordsResponse>(
            client_->EvalRequest(*request));
    if (!response) {
      throw ModelBackendException("Unsupported response from client in "
                                  "ServerModelBackend::WriteRecords.");
    }
    id = response->id();
  } catch (ProtocolException &e) {
    throw ModelBackendException(&e);
  }
}

void ServerModelBackend::InitSchema(
    const std::vector<RecordP> &/*records*/)
    throw(ModelBackendException) {
  
}

void ServerModelBackend::DeleteEntry(
    ContraptionID &id)
    throw(ModelBackendException) {
  message::DeleteEntryRequestP request(new message::DeleteEntryRequest());
  request->set_table(table_name_);
  request->set_id(static_cast<google::protobuf::uint32>(id));
  try {
    message::DeleteEntryResponseP response
        = boost::dynamic_pointer_cast<message::DeleteEntryResponse>(
            client_->EvalRequest(*request));
    if (!response) {
      throw ModelBackendException("Unsupported response from client in "
                                  "ServerModelBackend::DeleteEntry.");
    }
  } catch (ProtocolException &e) {
    throw ModelBackendException(&e);
  }
  id = Contraption::kNewID;
}

auto_ptr< vector<ContraptionID> > ServerModelBackend::Select(
    FilterCP filter)
    throw(ModelBackendException) {
  message::SelectRequestP request(new message::SelectRequest());
  request->set_table(table_name_);
  *request->mutable_filter() = *GetFilter(*filter);
  try {
    message::SelectResponseP response
        = boost::dynamic_pointer_cast<message::SelectResponse>(
            client_->EvalRequest(*request));
    if (!response) {
      throw ModelBackendException("Unsupported response from client in "
                                  "ServerModelBackend::DeleteEntry.");
    }
    auto_ptr< vector<ContraptionID> > ret(
        new vector<ContraptionID>(response->contraption_id_size()));
    for (size_t i = 0, end = ret->size(); i < end; ++i) {
      (*ret)[i] = response->contraption_id(static_cast<int>(i));
    }
    return ret;
  } catch (ProtocolException &e) {
    throw ModelBackendException(&e);
  }

}

