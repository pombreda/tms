#ifndef _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
#define _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
#ifdef WIN32
#include <windows.h>
#endif

// std
#include <istream>
#include <ostream>
#include <vector>
#include <map>
// boost
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/scoped_array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
// log4cplus
#include <log4cplus/logger.h>
// protobuf
#include <google/protobuf/io/zero_copy_stream_impl.h>
// common
#include <protocol/message.hpp>
#include <protocol/protocol_exception.hpp>
#include <rtti/typeinfo.hpp>

namespace tms {
namespace common {
namespace protocol {

class Protocol {
 public:
  Protocol()
      throw();
  Protocol(const Protocol &other)
      throw();
  template<class Message>
  // Do not add several MessageClass multiple times;
  void AddMessageClass()
      throw(ProtocolException);
  void Initialize()
      throw();
  bool IsInitialized()
      throw();
  MessageP ReadMessage(std::istream &sin)
      throw(ProtocolException);
  typedef boost::function<void (MessageP, 
                                ProtocolExceptionP exception)> AsyncReadHandler;
  typedef boost::function<void (ProtocolExceptionP exception)> AsyncWriteHandler;
  template <class AsyncReadStream>
  void AsyncReadMessage(AsyncReadStream &stream, AsyncReadHandler handler);
 
  template <class SyncReadStream>
  MessageP SyncReadMessage(SyncReadStream &stream);

  template <class AsyncWriteStream>
  void AsyncWriteMessage(AsyncWriteStream &stream, MessageP message, 
                         AsyncWriteHandler handler);

  template <class AsyncWriteStream>
  void AsyncWriteMessage(AsyncWriteStream &stream, MessageP message);

  template <class SyncWriteStream>
  void SyncWriteMessage(SyncWriteStream &stream, const Message &message);


  void WriteMessage(std::ostream &sout, 
                    const Message &message)
      throw(ProtocolException);  
    
  virtual ~Protocol() 
      throw() {}
 private:
  void DummyHandler (ProtocolExceptionP exception)
      throw(ProtocolException);
  static log4cplus::Logger logger_;
  class AsyncHelper;
  Protocol& operator= (const Protocol &other);

  class MessageHelper;

  template <class Message>
  class MessageHelperT;
  
  typedef boost::shared_ptr<const MessageHelper> MessageHelperCP;
  bool initialized_;
  typedef boost::unordered_map<rtti::TypeInfo, MessageHelperCP> HelpersMap;
  HelpersMap helpers_by_type_info_;
  std::vector<MessageHelperCP> helpers_;
};

typedef boost::shared_ptr<Protocol> ProtocolP;

}
}
}
#include "protocol_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
