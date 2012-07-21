#include "server.hpp"
// boost
#include <boost/bind.hpp>

using namespace std;
using namespace tms::common::protocol;

Server::Server(iostream &stream, ProtocolP protocol)
    throw()  :
    running_(false),
    stream_(stream),
    protocol_(protocol) {}  

void Server::Listen() 
    throw(ServerException) {
  try {
    listen_thread_.reset(new boost::thread(
        boost::bind(&Server::ListenThread, this)));
    running_ = true;
  } catch(boost::thread_resource_error &e) {
    throw ServerException(&e);
  }
}

void Server::Stop()
    throw(ServerException) {
  running_ = false;
  try {
    listen_thread_->join();
  } catch (boost::thread_interrupted e) {
    throw ServerException("Tread was interrupted in Server::Stop.");
  }
}

bool Server::IsListening()
    throw() {  
  return running_;
}

Server::~Server()
    throw() {}

void Server::ListenThread()
    throw() {
  while (running_) {
    boost::this_thread::disable_interruption di;
    MessageP message = protocol_->ReadMessage(stream_);
    protocol_->WriteMessage(stream_, *Eval(*message));
  }
}

MessageP Server::Eval(const Message &message) 
    throw(ServerException){
  return MessageP();
}
    
