// boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
using namespace std; //oops

namespace tms {
namespace common {
namespace protocol {

class Protocol::MessageHelper {
 public:
  MessageHelper(size_t id) : id_(id) {}
  virtual bool Accept(const Message &message) const = 0;
  virtual MessageP ReadMessage(std::istream &sin, uint32_t size) const = 0;
  virtual MessageP ReadMessage(const void *data, uint32_t size) const = 0;
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
  virtual MessageP ReadMessage(std::istream &sin, uint32_t size) const {
    MessageP ret(new Message());
    google::protobuf::io::IstreamInputStream stream(&sin, static_cast<int>(size));
    ret->ParseFromBoundedZeroCopyStream(&stream, static_cast<int>(size));
    return ret;
  }

  virtual MessageP ReadMessage(const void *data, uint32_t size) const {
    MessageP ret(new Message());
    google::protobuf::io::ArrayInputStream stream(data, static_cast<int>(size), 
                                                  static_cast<int>(size));
    ret->ParseFromZeroCopyStream(&stream);
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

template <class AsyncReadStream>
void Protocol::AsyncReadMessage(AsyncReadStream &stream, 
                                AsyncReadHandler handler) {
  uint32_t *buff = new uint32_t[2];
  boost::asio::async_read(stream, boost::asio::buffer(buff, sizeof(uint32_t) * 2),
                          boost::bind(&Protocol::AsyncReadHeader<AsyncReadStream>, 
                                      this, 
                                      boost::asio::placeholders::error, 
                                      boost::ref(stream),
                                      buff, 
                                      handler));
}

template <class AsyncWriteStream>
void Protocol::AsyncWriteMessage(AsyncWriteStream &stream, 
                                 AsyncWriteHandler handler) {
}

template <class AsyncReadStream>
void Protocol::AsyncReadHeader(const boost::system::error_code &ec,
                               AsyncReadStream &stream, uint32_t *buff,
                               AsyncReadHandler handler) {
  if (ec) {
    delete[] buff;
    handler(MessageP(), ProtocolExceptionP(
        new ProtocolException("IO error in Protocol::AsyncReadHandler.")));
  } else {
    uint32_t id = buff[0];
    uint32_t size = buff[1];
    delete[] buff;
    char *newbuff = new char[size];
    boost::asio::async_read(stream, boost::asio::buffer(newbuff, size),
                            boost::bind(&Protocol::AsyncReadBody<AsyncReadStream>, 
                                        this,
                                        boost::asio::placeholders::error, 
                                        newbuff, 
                                        id,
                                        size,
                                        handler));
  }
}

template <class AsyncReadStream>
void Protocol::AsyncReadBody(const boost::system::error_code &ec,
                             char *buff,
                             uint32_t id, uint32_t size,
                             AsyncReadHandler handler) {
  if (ec) {
    delete[] buff;
    handler(MessageP(), ProtocolExceptionP(
        new ProtocolException("IO error in Protocol::AsyncReadHandler.")));
  } else {
    ProtocolExceptionP exception;
    MessageP message;
    try {
      message = helpers_[id]->ReadMessage(buff, size);
    } catch (ProtocolException &e) {
      exception.reset(new ProtocolException(e));
    }
    delete[] buff;
    handler(message, exception);
  }
}
/*
template <class AsyncWriteStream>
void Protocol::AsyncWriteMessage(AsyncWriteStream &stream,
                                 MessageP message,
                                 AsyncWriteHandler handler) {
  char *buff = new char[8 + ];
  delete[] buff;
}
*/
    
}
}
}
