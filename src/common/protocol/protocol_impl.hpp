namespace tms {
namespace common {
namespace protocol {

class Protocol::MessageHelper {
 public:
  MessageHelper(size_t id) : id_(id) {}
  virtual bool Accept(const Message &message) const = 0;
  virtual MessageP ReadMessage(std::istream &sin) const = 0;
  size_t id() const {return id_;}
  virtual ~MessageHelper() {}
 private: 
  size_t id_;
};

template <class Message>
class Protocol::MessageHelperT : public MessageHelper  {
 public:
  MessageHelperT(size_t id) : MessageHelper(id) {}
  virtual bool Accept(const ::tms::common::protocol::Message &message) const {
    return dynamic_cast<const Message*>(&message);
  }
  virtual MessageP ReadMessage(std::istream &sin) const {
    MessageP ret(new Message());
    uint32_t size;
    sin.read(static_cast<char*>(static_cast<void*>(&size)), 4);
    google::protobuf::io::IstreamInputStream stream(&sin, size);
    ret->ParseFromBoundedZeroCopyStream(&stream, static_cast<int>(size));
    return ret;
  }
};

template<class Message>
void Protocol::AddMessageClass() 
    throw(ProtocolException) {
  BOOST_MPL_ASSERT(
      (boost::is_base_of<
       tms::common::protocol::Message,
       Message>));
  if (IsInitialized()) {
    throw ProtocolException("Protocol::AddMessageClass called for protocol "
                            "which is already initialized.");
  }
  helpers_by_type_info[rtti::TypeID<Message>()]
      =MessageHelperCP(new MessageHelperT<Message>(helpers_.size()));
  helpers_.push_back(
      MessageHelperCP(new MessageHelperT<Message>(helpers_.size())));
}

}
}
}
