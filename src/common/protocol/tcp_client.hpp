#ifndef _TMS_COMMON_PROTOCOL__TCP_CLIENT_HPP_
#define _TMS_COMMON_PROTOCOL__TCP_CLIENT_HPP_
// std
#include <istream>
// boost 
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/client.hpp>

namespace tms {
namespace common {
namespace protocol {

class TCPClient : public Client {
 public:
  TCPClient(std::iostream &stream, ProtocolP protocol) 
      throw():
      stream_(stream), 
      protocol_(protocol) {}
  MessageP EvalRequest(Message &message) 
      throw (ProtocolException) {
    protocol_->WriteMessage(stream_, message);
    return protocol_->ReadMessage(stream_);
  }
  virtual ~TCPClient() 
      throw(){}
 private:
  std::iostream &stream_;
  ProtocolP protocol_;
};


typedef boost::shared_ptr<TCPClient> TCPClientP;
}
}
}
#endif // _TMS_COMMON_PROTOCOL__TCP_CLIENT_HPP_
