#include "request_processor.hpp"
// common
#include <protocol/server.hpp>
using namespace tms::common::protocol;
log4cplus::Logger RequestProcessor::logger_ = log4cplus::Logger
    ::getInstance(LOG4CPLUS_TEXT("tms::common::protocol::RequestProcessor"));

RequestProcessor::RequestProcessor(Server &server) :
    server_(&server) {
}

RequestProcessor::RequestProcessor() :
    server_() {
}

Server& RequestProcessor::server() const
    throw() {
  return *server_;
}

void RequestProcessor::set_server(Server &server) 
    throw() {
  server_ = &server;
}
