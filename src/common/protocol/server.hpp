#ifndef _TMS_COMMON_PROTOCOL__SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_HPP_

// std
#include <istream>
// boost 
#include <boost/signals.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/server_exception.hpp>
namespace tms {
namespace common {
namespace protocol {

class Server {
 public:
  Server(std::iostream &stream, ProtocolP protocol)
      throw();
  void Listen()
      throw(ServerException);
  void Stop()
      throw(ServerException);
  bool IsListening()
      throw();
  virtual ~Server()
      throw();
 private:
  void ListenThread()
      throw();
  MessageP Eval(const Message &message)
      throw(ServerException);
  bool running_;
  std::auto_ptr<boost::thread> listen_thread_;
  std::iostream &stream_;
  ProtocolP protocol_;
};

}
}
}  
#endif // _TMS_COMMON_PROTOCOL__SERVER_HPP_
