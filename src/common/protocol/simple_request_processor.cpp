#include "simple_request_processor.hpp"
// std
#include <sstream>

using namespace tms::common::protocol;
using namespace std;

SimpleRequestProcessor::SimpleRequestProcessor(Server &server) :
    RequestProcessor(server),
    handlers_map_() {
}

SimpleRequestProcessor::SimpleRequestProcessor() :
    RequestProcessor(),
    handlers_map_() {
}

RequestProcessorP SimpleRequestProcessor::Duplicate() const {
  return RequestProcessorP(new SimpleRequestProcessor(*this));
}

MessageP SimpleRequestProcessor::Eval(const Message &message, Server &server)
    throw(ServerException) {
  HandlersMap::const_iterator it = handlers_map_.find(rtti::TypeID(message));
  if (it == handlers_map_.end()) {
      ostringstream msg;
      msg << "Unsupported Message in SimpleRequestProcessor::Eval. "
          << "Type: '"  << typeid(message).name() << "'.";      
      throw(ServerException(msg.str()));
  }
  return (it->second)(message, server);
}
