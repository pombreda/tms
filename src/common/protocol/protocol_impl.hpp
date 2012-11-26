// boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
// log4cplus
#include <log4cplus/loggingmacros.h>
// common
#include <protocol/message/disconnect_request.hpp>
#include <string/string.hpp>

namespace boost {
  inline void intrusive_ptr_add_ref(tms::common::protocol::Protocol* model) {
    ++model->ref_count_;
  }
  inline void intrusive_ptr_release(tms::common::protocol::Protocol* model) {
    --model->ref_count_;
  }
}


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
    if (size) {
      google::protobuf::io::IstreamInputStream stream(&sin, static_cast<int>(size));
      ret->ParseFromBoundedZeroCopyStream(&stream, static_cast<int>(size));
    }
    return ret;
  }

  virtual MessageP ReadMessage(const void *data, uint32_t size) const {
    MessageP ret(new Message());
    if (size) {
      google::protobuf::io::ArrayInputStream stream(data, static_cast<int>(size), 
                                                    static_cast<int>(size));
      ret->ParseFromZeroCopyStream(&stream);
    }
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
  helpers_by_type_info_[rtti::TypeID<Message>()]
      =MessageHelperCP(new MessageHelperT<Message>(helpers_.size()));
  helpers_.push_back(
      MessageHelperCP(new MessageHelperT<Message>(helpers_.size())));
}


class Protocol::AsyncHelper {
  typedef boost::shared_ptr<Protocol::AsyncHelper> AsyncHelperP;
 public:
  AsyncHelper(ProtocolP protocol) :
      buf(),
      protocol_(protocol) {}

  template <class AsyncReadStream>
  void AsyncReadMessage(AsyncReadStream &stream, 
                        AsyncReadHandler handler) {
    LOG4CPLUS_DEBUG(logger_, 
                    string::WStringFromUTF8String(
  	 	      "Reading Message Asynchronously"));
    AsyncHelperP ptr(this);
    buf.reset(new uint8_t[sizeof(uint32_t) * 2]);
    boost::asio::async_read(stream, boost::asio::buffer(&buf[0], 
                                                        sizeof(uint32_t) * 2),
                            boost::bind(&AsyncHelper
                                        ::AsyncReadHeader<AsyncReadStream>, 
                                        this, 
                                        boost::asio::placeholders::error, 
                                        boost::ref(stream),
                                        handler,
                                        ptr));
  }

  template <class AsyncWriteStream>
  void AsyncWriteMessage(AsyncWriteStream &stream,
                         MessageP message,
                         AsyncWriteHandler handler) {
    LOG4CPLUS_DEBUG(logger_, 
                    string::WStringFromUTF8String(
           	      "Writing Message Asynchronously"));

    AsyncHelperP ptr(this);
    buf.reset(new uint8_t[sizeof(uint32_t) * 2]);
    HelpersMap::const_iterator it 
        = protocol_->helpers_by_type_info_.find(rtti::TypeID(*message));
    if (it == protocol_->helpers_by_type_info_.end()) {
      std::ostringstream msg;
      msg << "Unknown message type in Protocol::AsyncWriteMessage "
          << "type = '" << typeid(message).name() << "'.";
      throw ProtocolException(msg.str());
    }
  
    reinterpret_cast<uint32_t*>(&buf[0])[0] 
        = static_cast<uint32_t>(it->second->id());
    reinterpret_cast<uint32_t*>(&buf[0])[1] 
        = static_cast<uint32_t>(message->ByteSize());
    boost::asio::async_write(stream, boost::asio::buffer(&buf[0], 
                                                         sizeof(uint32_t) * 2),
                             boost::bind(&AsyncHelper
                                         ::AsyncWriteHeader<AsyncWriteStream>, 
                                         this, 
                                         boost::asio::placeholders::error, 
                                         boost::ref(stream),
                                         message,
                                         handler,
                                         ptr));
  }

 private:
  template <class AsyncReadStream>
  void AsyncReadHeader(const boost::system::error_code &ec,
                       AsyncReadStream &stream,
                       AsyncReadHandler handler,
                       AsyncHelperP ptr) {
    if (ec) {
      if (ec == boost::asio::error::eof) {
        handler(MessageP(new(message::DisconnectRequest)), ProtocolExceptionP());
      } else {
        handler(MessageP(), ProtocolExceptionP(
            new ProtocolException("IO error in Protocol::"
                                  "AsyncHelper::AsyncReadHeader."
                                  + boost::lexical_cast<std::string>(ec)
                                  + boost::lexical_cast<std::string>(boost::asio::error::eof))));
      }
    } else {
      uint32_t id = reinterpret_cast<uint32_t*>(&buf[0])[0];
      uint32_t size = reinterpret_cast<uint32_t*>(&buf[0])[1];
      LOG4CPLUS_DEBUG(logger_, 
                      string::WStringFromUTF8String(
                        "Header read: id = "
		        + boost::lexical_cast<std::string>(id)
		        + ", size = "
		        + boost::lexical_cast<std::string>(size)));
      buf.reset(new uint8_t[size]);
      boost::asio::async_read(stream, boost::asio::buffer(&buf[0], size),
                              boost::bind(&AsyncHelper
                                          ::AsyncReadBody<AsyncReadStream>, 
                                          this,
                                          boost::asio::placeholders::error, 
                                          id,
                                          size,
                                          handler,
                                          ptr));
    }
  }

  template <class AsyncReadStream>
  void AsyncReadBody(const boost::system::error_code &ec,
                     uint32_t id, uint32_t size,
                     AsyncReadHandler handler,
                     AsyncHelperP /*ptr*/) {
    if (ec) {
      handler(MessageP(), ProtocolExceptionP(
          new ProtocolException("IO error in Protocol::"
                                "AsyncHelper::AsyncReadBody.")));
    } else {
      ProtocolExceptionP exception;
      MessageP message;
      try {
        message = protocol_->helpers_[id]->ReadMessage(&buf[0], size);
        LOG4CPLUS_DEBUG(logger_, 
                        string::WStringFromUTF8String(
				       "Message of type \"" 
                                       + rtti::TypeID(*message).name()
                                       + "\" read"));        
      } catch (ProtocolException &e) {
        exception.reset(new ProtocolException(e));
      }
      handler(message, exception);
    }
  }
  

  template <class AsyncWriteStream>
  void AsyncWriteHeader(const boost::system::error_code &ec,
                        AsyncWriteStream &stream, 
                        MessageP message,
                        AsyncWriteHandler handler,
                        AsyncHelperP ptr) {
    if (ec) {
      handler(ProtocolExceptionP(
          new ProtocolException("IO error in Protocol::AsyncWriteHeader.")));
    } else {
      LOG4CPLUS_DEBUG(logger_, 
                      string::WStringFromUTF8String("Header written: id = " 
                                     + boost::lexical_cast<std::string>(
                                         reinterpret_cast<uint32_t*>(&buf[0])[0])
                                     + ", size = "
                                     + boost::lexical_cast<std::string>(
                                         reinterpret_cast<uint32_t*>(&buf[0])[1])));

      uint32_t size = reinterpret_cast<uint32_t*>(&buf[0])[1];
      if (size == 0) {
        LOG4CPLUS_DEBUG(logger_, 
                        string::WStringFromUTF8String("0-size"));
                
        AsyncWriteBody<AsyncWriteStream>(boost::system::error_code(), 
                                         stream,
                                         message,
                                         handler,
                                         ptr);
        return;
      }
      buf.reset(new uint8_t[size]);
      google::protobuf::io::ArrayOutputStream gstream(&buf[0], 
                                                      static_cast<int>(size));
      message->SerializeToZeroCopyStream(&gstream);
      boost::asio::async_write(stream, boost::asio::buffer(&buf[0], size),
                               boost::bind(&AsyncHelper
                                           ::AsyncWriteBody<AsyncWriteStream>, 
                                           this,
                                           boost::asio::placeholders::error, 
                                           boost::ref(stream),
                                           message,
                                           handler,
                                           ptr));
    }
  } 

  template <class AsyncWriteStream>
  void AsyncWriteBody(const boost::system::error_code &ec,
                      AsyncWriteStream &/*stream*/, 
                      MessageP message,
                      AsyncWriteHandler handler,
                      AsyncHelperP /*ptr*/) {
    ProtocolExceptionP exception;
    if (ec) {
      exception.reset(new ProtocolException("IO error in Protocol::AsyncWriteBody"));
    } else {
      LOG4CPLUS_DEBUG(logger_, 
                      string::WStringFromUTF8String("Message of type \"" 
                                     + rtti::TypeID(*message).name()
                                     + "\" written"));        
    }
      
    handler(exception);
  }

 private:
  boost::scoped_array<uint8_t> buf;
  ProtocolP protocol_;
};

template <class AsyncReadStream>
void Protocol::AsyncReadMessage(AsyncReadStream &stream, 
                                AsyncReadHandler handler) {
  (new AsyncHelper(ProtocolP(this)))->AsyncReadMessage(stream, handler);
}

template <class AsyncWriteStream>
void Protocol::AsyncWriteMessage(AsyncWriteStream &stream,
                                 MessageP message,
                                 AsyncWriteHandler handler) {
  (new AsyncHelper(ProtocolP(this)))->AsyncWriteMessage(stream, message, handler);
}

template <class AsyncWriteStream>
void Protocol::AsyncWriteMessage(AsyncWriteStream &stream,
                                 MessageP message) {
  (new AsyncHelper(ProtocolP(this)))->AsyncWriteMessage(stream, 
                                              message, 
                                              boost::bind(&Protocol::DummyHandler, 
                                                          this, 
                                                          _1));
}

template <class SyncWriteStream>
void Protocol::SyncWriteMessage(SyncWriteStream &stream,
                                const Message &message) {
  boost::scoped_array<uint8_t> buf;
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String("Writing Message Synchronously"));
  buf.reset(new uint8_t[sizeof(uint32_t) * 2]);
  HelpersMap::const_iterator it 
        = helpers_by_type_info_.find(rtti::TypeID(message));
  if (it == helpers_by_type_info_.end()) {
    std::ostringstream msg;
    msg << "Unknown message type in Protocol::SyncWriteMessage "
        << "type = '" << typeid(message).name() << "'.";
    throw ProtocolException(msg.str());
  }
  
  reinterpret_cast<uint32_t*>(&buf[0])[0] 
      = static_cast<uint32_t>(it->second->id());
  reinterpret_cast<uint32_t*>(&buf[0])[1] 
      = static_cast<uint32_t>(message.ByteSize());
  boost::asio::write(stream, boost::asio::buffer(&buf[0], 
                                                 sizeof(uint32_t) * 2));
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String("Header written: id = " 
                                 + boost::lexical_cast<std::string>(
                                     reinterpret_cast<uint32_t*>(&buf[0])[0])
                                 + ", size = "
                                 + boost::lexical_cast<std::string>(
                                     reinterpret_cast<uint32_t*>(&buf[0])[1])));
  
  uint32_t size = reinterpret_cast<uint32_t*>(&buf[0])[1];
  if (size != 0) {
    buf.reset(new uint8_t[size]);
    google::protobuf::io::ArrayOutputStream gstream(&buf[0], 
                                                    static_cast<int>(size));
    message.SerializeToZeroCopyStream(&gstream);
    boost::asio::write(stream, boost::asio::buffer(&buf[0], size));
  }
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String("Message of type \"" 
						+ rtti::TypeID(message).name()
						+ "\" written"));        

}


template <class SyncReadStream>
MessageP Protocol::SyncReadMessage(SyncReadStream &stream) {
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String(
		    "Reading Message Synchronously"));

  boost::scoped_array<uint8_t> buf;
  buf.reset(new uint8_t[sizeof(uint32_t) * 2]);
  boost::asio::read(stream, 
                    boost::asio::buffer(&buf[0], sizeof(uint32_t) * 2));
  uint32_t id = reinterpret_cast<uint32_t*>(&buf[0])[0];
  uint32_t size = reinterpret_cast<uint32_t*>(&buf[0])[1];
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String(
                                 "Header read: id = " 
                                 + boost::lexical_cast<std::string>(id)
                                 + ", size = "
                                 + boost::lexical_cast<std::string>(size)));
  buf.reset(new uint8_t[size]);
  boost::asio::read(stream, boost::asio::buffer(&buf[0], size));
  MessageP message;
  message = helpers_[id]->ReadMessage(&buf[0], size);
  LOG4CPLUS_DEBUG(logger_, 
		  string::WStringFromUTF8String(
                                   "Message of type \"" 
                                   + rtti::TypeID(*message).name()
                                   + "\" read"));          
  return message;
}

}
}
}
