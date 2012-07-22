#include "server.hpp"
// boost
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace tms::common::protocol;

Server::Server() 
    throw():
    running_(false),
    listen_thread_(0),
    handlers_map_(new HandlersMap) {}  

Server::Server(HandlersMapP handlers_map) 
    throw():
    running_(false),
    listen_thread_(0),
    handlers_map_(handlers_map) {}  

void Server::Listen() 
    throw(ServerException) {
  try {
    running_ = true;
    listen_thread_.reset(new boost::thread(
        boost::bind(&Server::ListenThread, this)));
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
    throw ServerException("Thread was interrupted in Server::Stop.");
  }
}

bool Server::IsListening()
    throw() {  
  return running_;
}

Server::~Server()
    throw() {}
