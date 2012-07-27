#ifndef _TMS_COMMON_PROTOCOL__MODEL_BACKEND_REQUEST_PROCESSOR_HPP_
#define _TMS_COMMON_PROTOCOL__MODEL_BACKEND_REQUEST_PROCESSOR_HPP_
// std
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <protocol/message.hpp>
#include <protocol/message/read_records_request.hpp>
#include <protocol/message/read_records_response.hpp>
#include <protocol/message/write_records_request.hpp>
#include <protocol/message/write_records_response.hpp>
#include <protocol/message/delete_entry_request.hpp>
#include <protocol/message/delete_entry_response.hpp>
#include <protocol/message/select_request.hpp>
#include <protocol/message/select_response.hpp>
#include <protocol/server_exception.hpp>
#include <protocol/request_processor.hpp>
#include <protocol/login_request_processor.hpp>

namespace tms {
namespace common {
namespace protocol {

class ModelBackendRequestProcessor : public LoginRequestProcessor {
 public:
  // Users must have "string name" and "string password_hash" fields.
  ModelBackendRequestProcessor(Server &server,
                               RequestProcessorP request_processor, 
                               contraption::ModelP users,
                               const contraption::SOCIDBScheme &scheme);
  ModelBackendRequestProcessor(RequestProcessorP request_processor, 
                               contraption::ModelP users,
                               const contraption::SOCIDBScheme &scheme);
  virtual RequestProcessorP Duplicate() const;
  virtual MessageP Eval(const Message&);
  virtual ~ModelBackendRequestProcessor() {}
 private:  
  message::ReadRecordsResponseP ReadRecords(
      const message::ReadRecordsRequest &request);
  message::WriteRecordsResponseP WriteRecords(
      const message::WriteRecordsRequest &request);
  message::DeleteEntryResponseP DeleteEntry(
      const message::DeleteEntryRequest &request);
  message::SelectResponseP Select(
      const message::SelectRequest &request);
  contraption::SOCIModelBackendP GetBackend(const std::string &table);
  contraption::SOCIDBScheme scheme_;
  typedef boost::unordered_map<std::string, 
                               contraption::SOCIModelBackendP> BackendMap;
  BackendMap backend_map_;
};

typedef boost::shared_ptr<ModelBackendRequestProcessor> 
ModelBackendRequestProcessorP;


}
}
}
#endif // _TMS_COMMON_PROTOCOL__MODEL_BACKEND_REQUEST_PROCESSOR_HPP_
