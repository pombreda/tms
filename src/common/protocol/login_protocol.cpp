#include "login_protocol.hpp"
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <protocol/message/error_response.hpp>
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;

LoginProtocol::LoginProtocol() 
    throw(ProtocolException): 
    Protocol() {
  AddClasses();
}

LoginProtocol::LoginProtocol(const Protocol &other) 
    throw(ProtocolException): 
    Protocol(other) {
  AddClasses();
}

void LoginProtocol::AddClasses() 
    throw(ProtocolException) {
  AddMessageClass<LoginResponse>();
  AddMessageClass<ErrorResponse>();
  AddMessageClass<LoginRequest>();
}
