#include "stream_server.hpp"

using namespace std;
using namespace tms::common::protocol;
StreamServer::StreamServer(StreamP stream, ProtocolP protocol)
    throw()  :
    Server(),
    stream_(stream),
    protocol_(protocol) {}

StreamServer::StreamServer(StreamP stream, ProtocolP protocol, 
                           HandlersMapP handlers_map)
    throw()  :
    Server(handlers_map),
    stream_(stream),
    protocol_(protocol) {}

void StreamServer::ListenThread()
    throw() {
  while (IsListening()) {
    boost::this_thread::disable_interruption di;
    MessageP message = protocol_->ReadMessage(*stream_);
    protocol_->WriteMessage(*stream_, *Eval(*message));
  }
}

MessageP StreamServer::Eval(const Message &message) 
    throw(ServerException) {
  HandlersMap::iterator it = handlers_map_->find(rtti::TypeID(message));
  if (it == handlers_map_->end()) {
    throw(ServerException("Unsupported Message in StreamServer::Eval."));
  }
  return (it->second)(message);
}
    
