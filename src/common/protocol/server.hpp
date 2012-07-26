#ifndef _TMS_COMMON_PROTOCOL__SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_HPP_

// std
#include <istream>
// boost 
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
// log4cplus
#include <log4cplus/logger.h>
// common
#include <protocol/protocol.hpp>
#include <protocol/server_exception.hpp>
#include <protocol/request_processor.hpp>
#include <rtti/typeinfo.hpp>
namespace tms {
namespace common {
namespace protocol {

class Server {
 public:
  Server(RequestProcessorP request_processor)
      throw();
  virtual void Listen()
      throw(ServerException);
  virtual void Stop()
      throw(ServerException);
  virtual void Wait()
      throw(ServerException);
  virtual bool IsListening()
      throw();
  virtual ~Server()
      throw();
 private:
  bool running_;
  std::auto_ptr<boost::thread> listen_thread_;
 protected:
  static log4cplus::Logger logger_;
  RequestProcessorP request_processor_;
  virtual void ListenThread()
      throw() {};
};

typedef boost::shared_ptr<Server> ServerP;

}
}
}  
#include "server_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__SERVER_HPP_
