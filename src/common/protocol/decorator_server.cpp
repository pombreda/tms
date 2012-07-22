#include "decorator_server.hpp"

using namespace tms::common::protocol;

DecoratorServer::DecoratorServer(ServerP server) :
    server_(server) {}

void DecoratorServer::Listen()
    throw(ServerException) {
  server_->Listen();
}

void DecoratorServer::Stop()
    throw(ServerException) {
  server_->Stop();
}

void DecoratorServer::Wait()
    throw(ServerException) {
  server_->Wait();
}

bool DecoratorServer::IsListening()
    throw() {
  return server_->IsListening();
}

void DecoratorServer::AddHandler(boost::function<MessageP (const Message&)> handler, 
                                 const rtti::TypeInfo &typeinfo)
    throw() {
  server_->AddHandler(handler, typeinfo);
}
