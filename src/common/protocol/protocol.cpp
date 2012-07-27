#include "protocol.hpp"
// boost
#include <boost/lexical_cast.hpp>
// log4cplus
#include <log4cplus/loggingmacros.h>
// std
#include <sstream>

using namespace std;
using namespace tms::common::protocol;

log4cplus::Logger 
Protocol::logger_ = log4cplus::Logger
    ::getInstance(LOG4CPLUS_TEXT("tms::common::protocol::Protocol"));


Protocol::Protocol() 
    throw() :
    initialized_(false),
    helpers_by_type_info_(),
    helpers_(0) {}

Protocol::Protocol(const Protocol &other)
    throw() :
    initialized_(false),
    helpers_by_type_info_(other.helpers_by_type_info_),
    helpers_(other.helpers_) {}

bool Protocol::IsInitialized() 
    throw() {
  return initialized_;
}

void Protocol::Initialize() 
    throw() {
  initialized_ = true;
}

MessageP Protocol::ReadMessage(std::istream &sin) 
    throw(ProtocolException) {
  if (!IsInitialized()) {
    throw ProtocolException("Protocol::ReadMessage "
                            "called for uninitialized "
                            "protocol.");
  }
  try {
    LOG4CPLUS_DEBUG(logger_, 
                    LOG4CPLUS_TEXT("Reading Message Synchronously"));
    uint32_t id;
    sin.read(static_cast<char*>(static_cast<void*>(&id)), 4);  
    if (id >= helpers_.size()) {
      ostringstream msg;
      msg << "Unknown message id in Protocol::ReadMessage "
          << "id = " << id << ".";
      throw ProtocolException(msg.str());
    }
    if (!sin.good()) {
      throw(ProtocolException("Error while reading message."));
    }
    uint32_t size;
    sin.read(static_cast<char*>(static_cast<void*>(&size)), 4);
    if (!sin.good()) {
      throw(ProtocolException("Error while reading message."));
    }
    LOG4CPLUS_DEBUG(logger_, 
                    LOG4CPLUS_TEXT("Header read: id = " 
                                   + boost::lexical_cast<string>(id)
                                   + ", size = "
                                   + boost::lexical_cast<string>(size)));
    MessageP ret = helpers_[id]->ReadMessage(sin, size);
    LOG4CPLUS_DEBUG(logger_, 
                    LOG4CPLUS_TEXT("Message of type \"" 
                    + rtti::TypeID(*ret).name()
                    + "\" read"));
    return ret;
  } catch (ProtocolException&) {
    throw;
  } catch (std::exception &e) {
    throw ProtocolException(&e, "Exception in Protocol::ReadMessage.");
  }
}

void Protocol::WriteMessage(std::ostream &sout, 
                            const Message &message)
    throw(ProtocolException) {
  if (!IsInitialized()) {
    throw ProtocolException("Protocol::WriteMessage "
                            "called for uninitialized "
                            "protocol.");
  }
  try {
    LOG4CPLUS_DEBUG(logger_, 
                    LOG4CPLUS_TEXT("Writing Message Synchronously"));
    HelpersMap::const_iterator it 
        = helpers_by_type_info_.find(rtti::TypeID(message));
    if (it == helpers_by_type_info_.end()) {
      ostringstream msg;
      msg << "Unknown message type in Protocol::WriteMessage "
          << "type = '" << typeid(message).name() << "'.";
      throw ProtocolException(msg.str());
    }
    uint32_t buf[2];
    uint32_t id = static_cast<uint32_t>(it->second->id());
    buf[0] = id;
    uint32_t size = static_cast<uint32_t>(message.ByteSize());
    buf[1] = size;
    sout.write(static_cast<char*>(static_cast<void*>(&buf[0])), sizeof(buf));
    sout.flush();
    LOG4CPLUS_DEBUG(logger_, 
                    LOG4CPLUS_TEXT("Header written: id = " 
                                   + boost::lexical_cast<string>(id)
                                   + ", size = "
                                   + boost::lexical_cast<string>(size)));
    google::protobuf::io::OstreamOutputStream stream(&sout);
    message.SerializeToZeroCopyStream(&stream);
    sout.flush();
    LOG4CPLUS_DEBUG(logger_, 
                    LOG4CPLUS_TEXT("Message of type \"" 
                    + rtti::TypeID(message).name()
                    + "\" written"));
  } catch (ProtocolException&) {
    throw;
  } catch (std::exception &e) {
    throw ProtocolException(&e, "Exception in Protocol::WriteMessage.");
  }
}

void Protocol::DummyHandler (ProtocolExceptionP exception) 
    throw(ProtocolException){
  if (exception) {
    throw(*exception);
  }
}
