#ifndef _TMS_COMMON_PROTOCOL__PATCHER_REQUEST_PROCESSOR_HPP_
#define _TMS_COMMON_PROTOCOL__PATCHER_REQUEST_PROCESSOR_HPP_
// std
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
// common
#include <protocol/message.hpp>
#include <protocol/message/patch_client_request.hpp>
#include <protocol/message/patch_client_response.hpp>
#include <protocol/message/patch_server_request.hpp>
#include <protocol/server_exception.hpp>
#include <protocol/request_processor.hpp>
#include <protocol/simple_request_processor.hpp>
#include <protocol/model_backend_request_processor.hpp>

namespace tms {
namespace common {
namespace protocol {

class PatcherRequestProcessor : public ModelBackendRequestProcessor {
 public:
  // Users must have "string name" and "string password_hash" fields.
  PatcherRequestProcessor(Server &server,
			  RequestProcessorP request_processor, 
			  contraption::ModelP users,
			  const contraption::SOCIDBScheme &scheme);
  PatcherRequestProcessor(RequestProcessorP request_processor, 
			  contraption::ModelP users,
			  const contraption::SOCIDBScheme &scheme);
  virtual RequestProcessorP Duplicate() const;
  virtual MessageP Eval(const Message&, Server &server);
  virtual ~PatcherRequestProcessor() {}
  static void Register(SimpleRequestProcessor &processor);
 private:  
  static MessageP PatchServer(
      const message::PatchServerRequest &request, 
      Server &server);
  static MessageP PatchClient(
      const message::PatchClientRequest &request,
      Server &server);
};

typedef boost::shared_ptr<PatcherRequestProcessor> 
PatcherRequestProcessorP;


}
}
}
#endif // _TMS_COMMON_PROTOCOL__PATCHER_REQUEST_PROCESSOR_HPP_
