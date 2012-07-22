#include "protocol.hpp"
// std
#include <sstream>

using namespace std;
using namespace tms::common::protocol;

Protocol::Protocol() 
    throw() :
    initialized_(false),
    helpers_by_type_info(),
    helpers_(0) {}

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
    return helpers_[id]->ReadMessage(sin, size);
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
    HelpersMap::const_iterator it = helpers_by_type_info.find(rtti::TypeID(message));
    if (it == helpers_by_type_info.end()) {
      ostringstream msg;
      msg << "Unknown message type in Protocol::ReadMessage "
          << "type = '" << typeid(message).name() << "'.";
      throw ProtocolException(msg.str());
    }
    uint32_t id = static_cast<uint32_t>(it->second->id());

    sout.write(static_cast<char*>(static_cast<void*>(&id)), 4);
    uint32_t size = static_cast<uint32_t>(message.ByteSize());
    sout.write(static_cast<char*>(static_cast<void*>(&size)), 4);
    google::protobuf::io::OstreamOutputStream stream(&sout);
    message.SerializeToZeroCopyStream(&stream);
    sout.flush();
  } catch (ProtocolException&) {
    throw;
  } catch (std::exception &e) {
    throw ProtocolException(&e, "Exception in Protocol::WriteMessage.");
  }
}
