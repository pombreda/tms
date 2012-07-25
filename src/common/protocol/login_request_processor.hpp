#ifndef _TMS_COMMON_PROTOCOL__LOGIN_REQUEST_PROCESSOR_HPP_
#define _TMS_COMMON_PROTOCOL__LOGIN_REQUEST_PROCESSOR_HPP_
// boost
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/field/simple_field.hpp>
#include <protocol/message.hpp>
#include <protocol/server_exception.hpp>
#include <protocol/request_processor.hpp>

namespace tms {
namespace common {
namespace protocol {
class LoginRequestProcessor;
typedef boost::shared_ptr<LoginRequestProcessor> LoginRequestProcessorP;

class LoginRequestProcessor : public RequestProcessor {
 public:
  // Users must have "string name" and "string password_hash" fields.
  LoginRequestProcessor(RequestProcessorP request_processor, 
                        contraption::ModelP users);
  virtual RequestProcessorP Duplicate() const;
  virtual MessageP Eval(const Message&);
  virtual ~LoginRequestProcessor() {}
 protected:
  RequestProcessorP request_processor_;
  contraption::ModelP users_;
  contraption::ContraptionP user_;
 private:  
  const contraption::SimpleFieldT<std::string>* name_;
  const contraption::SimpleFieldT<std::string>* password_hash_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__LOGIN_REQUEST_PROCESSOR_HPP_
