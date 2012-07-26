#include "socket_server.hpp"
// log4cplus
#include <log4cplus/loggingmacros.h>
// boost 
#include <boost/bind.hpp>

using namespace std;
using namespace tms::common::protocol;
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
    ListenThread();
  } else {
    LOG4CPLUS_WARN(logger_, 
                   LOG4CPLUS_TEXT(exception->message()));
  }
}

void SocketServer::ReadMessageHandler(MessageP message, 
                                      ProtocolExceptionP exception) {
  if (!exception) {
    MessageP ret = request_processor_->Eval(*message);
    protocol_->AsyncWriteMessage(*socket_, ret,
                                 boost::bind(&SocketServer::WriteMessageHandler,
                                             this,
                                             _1));
  } else {
    LOG4CPLUS_WARN(logger_, 
                   LOG4CPLUS_TEXT(exception->message()));
  }
}


