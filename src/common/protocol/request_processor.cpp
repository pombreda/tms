#include "request_processor.hpp"
using namespace tms::common::protocol;
log4cplus::Logger RequestProcessor::logger_ = log4cplus::Logger
    ::getInstance(LOG4CPLUS_TEXT("tms::common::protocol::RequestProcessor"));

RequestProcessor::RequestProcessor(ServerP server) :
    server_(server) {
}
