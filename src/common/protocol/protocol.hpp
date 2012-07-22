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
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
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
  template <class AsyncWriteStream>
  void AsyncWriteMessage(AsyncWriteStream &stream, MessageP message, AsyncWriteHandler handler);
  void WriteMessage(std::ostream &sout, 
                    const Message &message)
      throw(ProtocolException);
  virtual ~Protocol() 
      throw() {}
 private:
  template <class AsyncReadStream>
  void AsyncReadHeader(const boost::system::error_code &ec,
                       AsyncReadStream &stream, uint32_t *buff,
                       AsyncReadHandler handler);

  template <class AsyncWriteStream>
  void AsyncWriteHeader(const boost::system::error_code &ec,
                       AsyncReadStream &stream, uint32_t *buff,
                       AsyncReadHandler handler);


  template <class AsyncReadStream>
  void AsyncReadBody(const boost::system::error_code &ec,
                     char *buff,
                     uint32_t id, uint32_t size,
                     AsyncReadHandler handler);

  template <class AsyncWriteStream>
  void AsyncWriteBody(const boost::system::error_code &ec,
                      char *buff,
                      uint32_t id, uint32_t size,
                      AsyncReadHandler handler);

  class MessageHelper;

  template <class Message>
  class MessageHelperT;
  
  typedef boost::shared_ptr<const MessageHelper> MessageHelperCP;
  bool initialized_;
  typedef boost::unordered_map<rtti::TypeInfo, MessageHelperCP> HelpersMap;
  HelpersMap helpers_by_type_info;
  std::vector<MessageHelperCP> helpers_;
};

typedef boost::shared_ptr<Protocol> ProtocolP;

}
}
}
#include "protocol_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
