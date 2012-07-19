#ifndef _TMS_COMMON_PROTOCOL__CLIENT_HPP_
#define _TMS_COMMON_PROTOCOL__CLIENT_HPP_
// std
#include <istream>
// common
#include <protocol/protocol.hpp>

namespace tms {
namespace commont {
namespace protocol {

class Client {
 public:
  Client(std::iostream &stream, ProtocolP protocol) 
      throw():
      stream_(stream_), 
      protocol_(protocol) {}
  MessageP EvalRequest(Message &message) 
      throw (ProtocolException) {
    protocol_.WriteMessage(stream_, message);
    return protocol_.ReadMessage(stream_);
  }
  virtual ~Client() 
      throw(){}
 private:
  std::iostream &stream_;
  ProtocolP protocol_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__CLIENT_HPP_
