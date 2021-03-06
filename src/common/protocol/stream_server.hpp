#ifndef _TMS_COMMON_PROTOCOL__STREAM_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__STREAM_SERVER_HPP_

// std
#include <istream>
// boost 
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/server.hpp>
#include <protocol/server_exception.hpp>
#include <protocol/stream.hpp>
namespace tms {
namespace common {
namespace protocol {
class StreamServer;

typedef boost::shared_ptr<StreamServer> StreamServerP;

class StreamServer : public Server {
 public:
  StreamServer(StreamP stream, 
               ProtocolP protocol, 
               RequestProcessorP request_processor)
      throw();
  ~StreamServer()
      throw() {}
 private:
  StreamP stream_;
  ProtocolP protocol_;
 protected:
  virtual void ListenThread()
      throw();
};

}
}
}  
#endif // _TMS_COMMON_PROTOCOL__STREAM_SERVER_HPP_
