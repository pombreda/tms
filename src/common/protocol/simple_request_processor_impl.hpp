namespace tms {
namespace common {
namespace protocol {


template<class Message>
class SimpleRequestProcessor::FunctionWrapper {
 public:
  FunctionWrapper(boost::function<MessageP (const Message&, Server &server)> helper) : 
      helper_(helper) {
  }

  MessageP operator()(const protocol::Message& message, Server &server) 
      throw(ServerException) {
    const Message *message_ptr = dynamic_cast<const Message*>(&message);
    if (!message_ptr) {
      throw(ServerException("Incorrect message type in FunctionWrapper."));
    }
    return helper_(*message_ptr, server);
  }

 private:
  boost::function<MessageP (const Message&, Server &server)> helper_;
};

template<class Message>
class SimpleRequestProcessor::ShortFunctionWrapper {
 public:
  ShortFunctionWrapper(boost::function<MessageP (const Message&)> helper) : 
      helper_(helper) {
  }

  MessageP operator()(const protocol::Message& message, Server &server) 
      throw(ServerException) {
    const Message *message_ptr = dynamic_cast<const Message*>(&message);
    if (!message_ptr) {
      throw(ServerException("Incorrect message type in FunctionWrapper."));
    }
    return helper_(*message_ptr);
  }

 private:
  boost::function<MessageP (const Message&)> helper_;
};


template<class Message>
void SimpleRequestProcessor::AddHandler(
    boost::function<MessageP (const Message&, Server &server)> handler) 
    throw() {
  handlers_map_[rtti::TypeID<const Message&>()] 
      = FunctionWrapper<Message>(handler);
}

template<class Message>
void SimpleRequestProcessor::AddHandler(MessageP (*handler)(const Message&, Server &server)) 
    throw() {
  AddHandler(static_cast< boost::function<MessageP (const Message&)> >(handler));
}

template<class Message>
void SimpleRequestProcessor::AddHandler(
    boost::function<MessageP (const Message&)> handler) 
    throw() {
  handlers_map_[rtti::TypeID<const Message&>()] 
      = ShortFunctionWrapper<Message>(handler);
}

template<class Message>
void SimpleRequestProcessor::AddHandler(MessageP (*handler)(const Message&)) 
    throw() {
  AddHandler(static_cast< boost::function<MessageP (const Message&)> >(handler));
}


}
}
}
