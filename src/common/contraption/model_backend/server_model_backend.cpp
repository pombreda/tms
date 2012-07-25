#include "server_model_backend.hpp"
// boost
#include <boost/foreach.hpp>
// common
#include <protocol/message/record.hpp>
#include <protocol/message/read_records_request.hpp>
#include <protocol/message/read_records_response.hpp>
#include <contraption/contraption.hpp>
#include <contraption/record.hpp>

using namespace tms::common::contraption;
using namespace tms::common::protocol;
using tms::common::protocol::message::GetFreeRecord;
using tms::common::protocol::message::GetValue;

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
    ostringstream msg;
    msg << "Unsupported record type int ServerModelBackend::ReadRecords. "
        << "Type: '"  << typeid(*record).name() << "'.";
    throw ModelBackendException(msg.str());    
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
}

void ServerModelBackend::InitSchema(
    const std::vector<RecordP> &records)
    throw(ModelBackendException) {
}

void ServerModelBackend::DeleteEntry(
    ContraptionID &id)
    throw(ModelBackendException) {
}

auto_ptr< vector<ContraptionID> > ServerModelBackend::Select(
    FilterCP filter)
    throw(ModelBackendException) {
}
