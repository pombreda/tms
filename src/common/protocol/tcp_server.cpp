#include "tcp_server.hpp"
// boost
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
// log4cplus
#include <log4cplus/loggingmacros.h>
// common
#include <protocol/socket_server.hpp>
using boost::asio::ip::tcp;
using namespace tms::common::protocol;
using namespace std;

TCPServer::TCPServer(tcp::endpoint endpoint, ProtocolP protocol, RequestProcessorP request_processor) 
    throw():
    Server(request_processor),
    io_service_(),
    acceptor_(io_service_, endpoint),
    listeners_(0),
    protocol_(protocol) {}

TCPServer::~TCPServer() 
    throw() {}

void TCPServer::Stop() 
    throw(ServerException) {
  io_service_.stop();
  Server::Stop(); 
  BOOST_FOREACH(ServerP listener, listeners_) {
    listener->Stop();
  }
}

void TCPServer::HandleAccept(SocketP socket, 
                             const boost::system::error_code &ec) 
    throw() {
  if (!ec) {    
    listeners_.push_back(ServerP(
        new SocketServer(socket, protocol_, 
                         request_processor_->Duplicate())));
    listeners_.back()->Listen();
  } else {
    LOG4CPLUS_WARN(logger_, 
                   LOG4CPLUS_TEXT(string("Error in TCPServer::HandleAccept: ") 
                                  + ec.category().name() + " " 
                                  + boost::lexical_cast<string>(ec.value())));
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
