#ifndef _TMS_COMMON_PROTOCOL__TCP_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__TCP_SERVER_HPP_
// std
#include <vector>
// boost
#include <boost/asio.hpp>
// common
#include <protocol/server.hpp>
#include <protocol/protocol.hpp>
#include <protocol/socket.hpp>
namespace tms {
namespace common {
namespace protocol {

class TCPServer : public Server {
 public:
  TCPServer(boost::asio::ip::tcp::endpoint endpoint, 
            ProtocolP protocol, 
            RequestProcessorP request_processor)
      throw();
  virtual void Stop()
      throw(ServerException);
  virtual ~TCPServer()
      throw();
 private:
  void StartAccept()
      throw();
  void HandleAccept(SocketP socket, 
                    const boost::system::error_code &error)
      throw();
  virtual void ListenThread()
      throw();
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::vector<ServerP> listeners_;  
  ProtocolP protocol_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__TCP_SERVER_HPP_
