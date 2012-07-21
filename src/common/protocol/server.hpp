#ifndef _TMS_COMMON_PROTOCOL__SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_HPP_

// std
#include <istream>
// boost 
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/server_exception.hpp>
#include <rtti/typeinfo.hpp>
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
  template<class Message>
  void AddHandler(boost::function<MessageP (const Message&)> handler);
  virtual ~Server()
      throw();
 private:
  typedef boost::function<MessageP (const Message&)> HandlerFunction;
  typedef boost::unordered_map<rtti::TypeInfo, HandlerFunction> HandlersMap;
  void ListenThread()
      throw();
  MessageP Eval(const Message &message)
      throw(ServerException);
  bool running_;
  std::auto_ptr<boost::thread> listen_thread_;
  std::iostream &stream_;
  ProtocolP protocol_;
  HandlersMap handlers_map_;
};

}
}
}  
#include "server_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__SERVER_HPP_
