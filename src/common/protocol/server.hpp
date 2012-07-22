#ifndef _TMS_COMMON_PROTOCOL__SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_HPP_

// std
#include <istream>
// boost 
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/server_exception.hpp>
#include <rtti/typeinfo.hpp>
namespace tms {
namespace common {
namespace protocol {

class Server {
 public:
  Server()
      throw();
  virtual void Listen()
      throw(ServerException);
  virtual void Stop()
      throw(ServerException);
  virtual void Wait()
      throw(ServerException);
  virtual bool IsListening()
      throw();
  template<class Message>
  void AddHandler(boost::function<MessageP (const Message&)> handler)
      throw();
  template<class Message>
  void AddHandler(MessageP (*handler)(const Message&)) 
      throw();
  virtual ~Server()
      throw();
  virtual void AddHandler(boost::function<MessageP (const Message&)> handler, 
                          const rtti::TypeInfo &typeinfo)
      throw();
 private:
  template<class Message>
  class FunctionWrapper;
  bool running_;
  std::auto_ptr<boost::thread> listen_thread_;
 protected:
  virtual void ListenThread()
      throw() {};
  typedef boost::function<MessageP (const Message&)> HandlerFunction;
  typedef boost::unordered_map<rtti::TypeInfo, HandlerFunction> HandlersMap;
  typedef boost::shared_ptr<HandlersMap> HandlersMapP;
  Server(HandlersMapP handlers_map)
      throw();
  HandlersMapP handlers_map_;
};

typedef boost::shared_ptr<Server> ServerP;

}
}
}  
#include "server_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__SERVER_HPP_
