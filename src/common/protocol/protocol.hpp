#ifndef _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
#define _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
// std
#include <istream>
#include <ostream>
#include <vector>
#include <map>
// boost
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/scoped_array.hpp>
// protobuf
#include <google/protobuf/io/zero_copy_stream_impl.h>
// common
#include <protocol/message.hpp>
#include <protocol/protocol_exception.hpp>


namespace tms {
namespace common {
namespace protocol {

class Protocol {
 public:
  Protocol()
      throw();
  template<class Message>
  // Do not add several transaction of the same type;
  void AddMessageClass()
      throw(ProtocolException);
  void Initialize()
      throw();
  bool IsInitialized()
      throw();
  MessageP ReadMessage(std::istream &sin)
      throw(ProtocolException);
  void WriteMessage(std::ostream &sout, 
                    const Message &message)
      throw(ProtocolException);
  ~Protocol() {}
 private:
  class MessageHelper {
   public:
    virtual bool Accept(const Message &message) const = 0;
    virtual MessageP ReadMessage(std::istream &sin) const = 0;
    virtual ~MessageHelper() {}
 };

  template <class Message>
  class MessageHelperT : public MessageHelper {
   public:
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

  typedef boost::shared_ptr<const MessageHelper> MessageHelperCP;
  bool initialized_;
  std::vector<MessageHelperCP> helpers_;
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
  helpers_.push_back(MessageHelperCP(new MessageHelperT<Message>()));
}

typedef shared_ptr<Protocol> ProtocolP;

}
}
}
#endif // _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
