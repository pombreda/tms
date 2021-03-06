#include "socket_server.hpp"
// log4cplus
#include <log4cplus/loggingmacros.h>
// boost 
#include <boost/bind.hpp>
// common
#include <protocol/message/disconnect_request.hpp>
#include <protocol/message/error_response.hpp>
#include <string/string.hpp>

using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;
using namespace tms::common::string;

SocketServer::SocketServer(SocketP socket, 
                           ProtocolP protocol, 
                           RequestProcessorP request_processor)
    throw()  :    
    Server(request_processor),
    socket_(socket),
    protocol_(protocol) {}

void SocketServer::ListenThread()
    throw() {
  protocol_->AsyncReadMessage(*socket_,
                              boost::bind(&SocketServer::ReadMessageHandler, 
                                          this, 
                                          _1, 
                                          _2));
}

void SocketServer::WriteMessageHandler(ProtocolExceptionP exception) {
  if (!exception) {
    
  } else {
    LOG4CPLUS_WARN(logger_, 
                   WStringFromUTF8String(exception->message()));
  }
}

void SocketServer::ReadMessageHandler(MessageP message, 
                                      ProtocolExceptionP exception) {
  if (!exception) {
    if (dynamic_cast<message::DisconnectRequest*>(&*message)) {
      Stop();
      return;
    }
    ListenThread();
    try {
      MessageP ret = request_processor_->Eval(*message, *this);
      protocol_->AsyncWriteMessage(*socket_, ret,
                                   boost::bind(&SocketServer::WriteMessageHandler,
                                               this,
                                               _1));
      return;
    } catch (std::exception &e) {
      exception = ProtocolExceptionP(new ProtocolException(&e));
    }
  } else {
    ErrorResponseP ret = ErrorResponseP(new ErrorResponse());
    ret->set_status(ErrorResponse::kServerError);
    LOG4CPLUS_WARN(logger_, 
                   WStringFromUTF8String(exception->message()));
    protocol_->AsyncWriteMessage(*socket_, ret,
                                 boost::bind(&SocketServer::WriteMessageHandler,
                                             this,
                                             _1));
  }
}


