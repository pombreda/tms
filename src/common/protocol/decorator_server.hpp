#ifndef _TMS_COMMON_PROTOCOL__DECORATOR_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__DECORATOR_SERVER_HPP_
// common
#include <protocol/server.hpp>

namespace tms {
namespace common {
namespace protocol {

class DecoratorServer : public Server {
 public: 
  DecoratorServer(ServerP server);
  virtual void Listen()
      throw(ServerException);
  virtual void Stop()
      throw(ServerException);
  virtual void Wait()
      throw(ServerException);
  virtual bool IsListening()
      throw();
  virtual void AddHandler(boost::function<MessageP (const Message&)> handler, 
                          const rtti::TypeInfo &typeinfo)
      throw();
 private:
  ServerP server_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__DECORATOR_SERVER_HPP_
