#include "login_request_processor.hpp"
#include <protocol/message/login_request.pb.h>
#include <protocol/message/login_response.pb.h>
#include <protocol/message/error_response.pb.h>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/contraption_array.hpp>

using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;
using namespace tms::common::contraption;

LoginRequestProcessor::LoginRequestProcessor(RequestProcessorP request_processor, 
                                             ModelP users) :
    request_processor_(request_processor),
    users_(users),
    user_(),
    name_(dynamic_cast<const SimpleFieldT<string>*>(users->GetField("name"))),
    password_hash_(
        dynamic_cast<const SimpleFieldT<string>*>(
            users->GetField("password_hash"))) {
}

RequestProcessorP LoginRequestProcessor::Duplicate() {
  return RequestProcessorP(
      new LoginRequestProcessor(request_processor_->Duplicate(), 
                                users_));
}

MessageP LoginRequestProcessor::Eval(const Message &message) {
  const LoginRequest* request = dynamic_cast<const LoginRequest*>(&message);
  if (request) {
    ContraptionArrayP users = users_->Filter(And(
        Compare(name_, kEqual, request->name()),
        Compare(password_hash_, kEqual, request->password_hash())));
    if (users->size() > 0) {
      user_ = users->at(0);
      return MessageP(new LoginResponse());      
    }
  } else {
    if (user_) {
      return request_processor_->Eval(message);
    }
  }
  return MessageP(new ErrorResponse());
}
