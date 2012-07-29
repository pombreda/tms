#ifndef _TMS_COMMON_PROTOCOL__SOCKET_CLIENT_HPP_
#define _TMS_COMMON_PROTOCOL__SOCKET_CLIENT_HPP_
// boost 
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/client.hpp>
#include <protocol/socket.hpp>

namespace tms {
namespace common {
namespace protocol {


class SocketClient : public Client {
 public:
  SocketClient(SocketP socket, ProtocolP protocol) 
      throw():
      socket_(socket), 
      protocol_(protocol),
      ip_() {
    socket_->lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
  }

  SocketClient(std::string server, std::string port, ProtocolP protocol) 
      throw():
      socket_(), 
      protocol_(protocol),
      ip_() {
    ip_.reset(new boost::asio::io_service());
    
    boost::asio::ip::tcp::resolver resolver(*ip_);
    boost::asio::ip::tcp::resolver::query query(
    boost::asio::ip::tcp::v4(), server, port);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    socket_.reset(new Socket(*ip_));
    boost::asio::connect(*socket_, iterator);
    socket_->lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
  }

  MessageP EvalRequest(Message &message) 
      throw (ProtocolException) {
    protocol_->SyncWriteMessage(*socket_, message);
    return protocol_->SyncReadMessage(*socket_);
  }
  virtual ~SocketClient() 
      throw(){}
 private:
  SocketP socket_;
  ProtocolP protocol_;
  boost::shared_ptr< boost::asio::io_service > ip_;
};


typedef boost::shared_ptr<SocketClient> SocketClientP;
}
}
}
#endif // _TMS_COMMON_PROTOCOL__SOCKET_CLIENT_HPP_
