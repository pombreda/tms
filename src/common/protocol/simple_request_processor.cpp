#include "simple_request_processor.hpp"

using namespace tms::common::protocol;

SimpleRequestProcessor::SimpleRequestProcessor() :
    handlers_map_() {
}

RequestProcessorP SimpleRequestProcessor::Duplicate() {
  return RequestProcessorP(new SimpleRequestProcessor(*this));
}

MessageP SimpleRequestProcessor::Eval(const Message &message) 
    throw(ServerException) {
  HandlersMap::iterator it = handlers_map_.find(rtti::TypeID(message));
  if (it == handlers_map_.end()) {
    throw(ServerException("Unsupported Message in StreamServer::Eval."));
  }
  return (it->second)(message);
}
