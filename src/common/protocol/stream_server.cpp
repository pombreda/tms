#include "stream_server.hpp"

using namespace std;
using namespace tms::common::protocol;
StreamServer::StreamServer(StreamP stream, ProtocolP protocol, 
                           RequestProcessorP request_processor)
    throw()  :
    Server(request_processor),
    stream_(stream),
    protocol_(protocol) {}

void StreamServer::ListenThread()
    throw() {
  while (IsListening()) {
    boost::this_thread::disable_interruption di;
    MessageP message = protocol_->ReadMessage(*stream_);
    protocol_->WriteMessage(*stream_, *request_processor_->Eval(*message));
  }
}

    
