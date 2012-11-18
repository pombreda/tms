#include "login_request_processor.hpp"
// log4cplus
#include <log4cplus/loggingmacros.h>
// common
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <protocol/message/error_response.hpp>
#include <protocol/server.hpp>
#include <string/string.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/contraption_array.hpp>

using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;
using namespace tms::common::contraption;
using namespace tms::common::string;

LoginRequestProcessor::LoginRequestProcessor(Server &server,
                                             RequestProcessorP request_processor, 
                                             ModelP users) :
  RequestProcessor(server),
  request_processor_(request_processor),
  users_(users),
  name_(dynamic_cast<const SimpleFieldT<std::string>*>(users->GetField("name"))),
  password_hash_(
		 dynamic_cast<const SimpleFieldT<std::string>*>(
								users->GetField("password_hash"))) {
}

LoginRequestProcessor::LoginRequestProcessor(RequestProcessorP request_processor, 
                                             ModelP users) :
  RequestProcessor(request_processor->server()),
  request_processor_(request_processor),
  users_(users),
  name_(dynamic_cast<const SimpleFieldT<std::string>*>(users->GetField("name"))),
  password_hash_(
		 dynamic_cast<const SimpleFieldT<std::string>*>(
								users->GetField("password_hash"))) {
}

RequestProcessorP LoginRequestProcessor::Duplicate() const {
  return RequestProcessorP(
			   new LoginRequestProcessor(*server_,
						     request_processor_->Duplicate(), 
						     users_));
}

MessageP LoginRequestProcessor::Eval(const Message &message, Server &server) {
  const LoginRequest* request = dynamic_cast<const LoginRequest*>(&message);

  if (request) {
    ContraptionArrayP users = users_->Filter(And(
						 Compare(name_, kEqual, request->name()),
						 Compare(password_hash_, kEqual, request->password_hash())));
    if (users->size() > 0) {
      server_->Set<ContraptionP>("user", users->at(0));
      LOG4CPLUS_INFO(logger_, 
                     WStringFromUTF8String("User " 
                                    + server_->Get<ContraptionP>("user")
                                    ->Get<std::string>("name")
                                    + " logged in"));  
      LoginResponseP response(new LoginResponse);
      response->set_admin(users->at(0)->Get<int>("admin"));
      response->set_secretair(users->at(0)->Get<int>("secretair"));
      return response;      
    } else {
      LOG4CPLUS_INFO(logger_, 
                     WStringFromUTF8String("User " + request->name()
                                    + " was rejected"));  
    }
  } else {
    if (server_->Check("user")) {
      return request_processor_->Eval(message, server);
    }
  }
  return MessageP(new ErrorResponse());
}
