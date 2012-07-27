#ifndef _TMS_COMMON_PROTOCOL__STREAM_CLIENT_HPP_
#define _TMS_COMMON_PROTOCOL__STREAM_CLIENT_HPP_
// std
#include <istream>
// boost 
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/client.hpp>
#include <protocol/stream.hpp>

namespace tms {
namespace common {
namespace protocol {

class StreamClient : public Client {
 public:
  StreamClient(StreamP stream, ProtocolP protocol) 
      throw():
      stream_(stream), 
      protocol_(protocol) {

  }
  MessageP EvalRequest(Message &message) 
      throw (ProtocolException) {
    protocol_->WriteMessage(*stream_, message);
    return protocol_->ReadMessage(*stream_);
  }
  virtual ~StreamClient() 
      throw(){}
 private:
  StreamP stream_;
  ProtocolP protocol_;
};


typedef boost::shared_ptr<StreamClient> StreamClientP;
}
}
}
#endif // _TMS_COMMON_PROTOCOL__STREAM_CLIENT_HPP_
