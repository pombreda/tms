#include "tcp_server.hpp"
// boost
#include <boost/foreach.hpp>
// common
#include <protocol/socket_server.hpp>
using boost::asio::ip::tcp;
using namespace tms::common::protocol;
using namespace std;

TCPServer::TCPServer(tcp::endpoint endpoint, ProtocolP protocol) 
    throw():
    io_service_(),
    acceptor_(io_service_, endpoint),
    listeners_(0),
    protocol_(protocol) {}

TCPServer::~TCPServer() 
    throw() {}

void TCPServer::Stop() 
    throw(ServerException){
  BOOST_FOREACH(ServerP listener, listeners_) {
    listener->Stop();
  }
  io_service_.stop();
  Server::Stop();
}

void TCPServer::HandleAccept(SocketP socket, 
                             const boost::system::error_code &error) 
    throw() {
  if (!error) {    
    listeners_.push_back(ServerP(
        new SocketServer(socket, protocol_, handlers_map_)));
    listeners_.back()->Listen();
  }
  StartAccept();
}

void TCPServer::StartAccept()
    throw() {
  SocketP socket(new tcp::socket(io_service_));
  acceptor_.async_accept(*socket,
                         boost::bind(&TCPServer::HandleAccept, this, socket,
                                     boost::asio::placeholders::error));  
}

void TCPServer::ListenThread() 
    throw() {
  StartAccept();
  io_service_.run();
}
