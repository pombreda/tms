#include "socket_server.hpp"
// boost 
#include <boost/bind.hpp>

using namespace std;
using namespace tms::common::protocol;
SocketServer::SocketServer(SocketP socket, ProtocolP protocol)
    throw()  :
    Server(),
    socket_(socket),
    protocol_(protocol) {}

SocketServer::SocketServer(SocketP socket, ProtocolP protocol, 
                           HandlersMapP handlers_map)
    throw()  :
    Server(handlers_map),
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
    // oops
  }
}

void SocketServer::ReadMessageHandler(MessageP message, 
                                      ProtocolExceptionP exception) {
  if (!exception) {
    MessageP ret = Eval(*message);
    protocol_->AsyncWriteMessage(*socket_, ret,
                                 boost::bind(&SocketServer::WriteMessageHandler,
                                             this,
                                             _1));
  } else {
    //oops
  }
}

MessageP SocketServer::Eval(const Message &message) 
    throw(ServerException) {
  HandlersMap::iterator it = handlers_map_->find(rtti::TypeID(message));
  if (it == handlers_map_->end()) {
    throw(ServerException("Unsupported Message in SocketServer::Eval."));
  }
  return (it->second)(message);
}

