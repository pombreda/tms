#include "server.hpp"
// boost
#include <boost/thread.hpp>
#include <boost/bind.hpp>
// log4cplus
#include <log4cplus/loggingmacros.h>
using namespace std;
using namespace tms::common::protocol;

log4cplus::Logger 
Server::logger_ = log4cplus::Logger
    ::getInstance(LOG4CPLUS_TEXT("tms::common::protocol::Server"));

Server::Server(RequestProcessorP request_processor) 
    throw():
    running_(false),
    listen_thread_(0),
    session_(),
    request_processor_(request_processor) {
  request_processor->set_server(*this);
}  

void Server::Listen() 
    throw(ServerException) {
  try {
    running_ = true;
    listen_thread_.reset(new boost::thread(
        boost::bind(&Server::ListenThread, this)));
  } catch(boost::thread_resource_error &e) {
    throw ServerException(&e);
  }
  LOG4CPLUS_INFO(logger_, 
                 LOG4CPLUS_TEXT("Server " 
                                + rtti::TypeID(*this).name() 
                                + " started"));
}

void Server::ListenHere() 
    throw(ServerException) {
  try {
    running_ = true;
    ListenThread();
  } catch(boost::thread_resource_error &e) {
    throw ServerException(&e);
  }
  LOG4CPLUS_INFO(logger_, 
                 LOG4CPLUS_TEXT("Server " 
                                + rtti::TypeID(*this).name() 
                                + " started"));
}


void Server::Stop()
    throw(ServerException) {
  try {
    if (listen_thread_.get()) {
      listen_thread_->join();
    }
  } catch (boost::thread_interrupted e) {
    throw ServerException("Thread was interrupted in Server::Stop.");
  }
  running_ = false;
  LOG4CPLUS_INFO(logger_, 
                 LOG4CPLUS_TEXT("Server " 
                                + rtti::TypeID(*this).name() 
                                + " stopped"));
}

void Server::Wait()
    throw(ServerException) {
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

bool Server::Check(const std::string &var) const
    throw() {
  LOG4CPLUS_DEBUG(logger_, 
                  LOG4CPLUS_TEXT("Checking " + var));

  return session_.count(var) != 0;
}

Server::~Server()
    throw() {}
