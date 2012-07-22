#ifndef _TMS_COMMON_PROTOCOL__SOCKET_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SOCKET_SERVER_HPP_

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
#include <protocol/socket.hpp>
namespace tms {
namespace common {
namespace protocol {
class SocketServer;

typedef boost::shared_ptr<SocketServer> SocketServerP;

class SocketServer : public Server {
 public:
  SocketServer(SocketP socket, ProtocolP protocol)
      throw();
  SocketServer(SocketP socket, ProtocolP protocol, HandlersMapP handlers_map)
      throw();
 private:
  void ReadMessageHandler(MessageP message, ProtocolExceptionP exception);
  void WriteMessageHandler(ProtocolExceptionP exception);
  virtual void ListenThread()
      throw();
  MessageP Eval(const Message &message)
      throw(ServerException);
  SocketP socket_;
  ProtocolP protocol_;
};

}
}
}  
#endif // _TMS_COMMON_PROTOCOL__SOCKET_SERVER_HPP_
