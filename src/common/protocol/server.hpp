#ifndef _TMS_COMMON_PROTOCOL__SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_HPP_
#include "server_fwd.hpp"
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
#include <protocol/session_exception.hpp>
#include <protocol/request_processor.hpp>
#include <contraption/field_type.hpp>
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
  virtual void ListenHere()
      throw(ServerException);
  virtual void Stop()
      throw(ServerException);
  virtual void Wait()
      throw(ServerException);
  virtual bool IsListening()
      throw();
  virtual ~Server()
      throw();
  bool Check(const std::string &var) const
      throw();
  template<class T>
  const T& Get(const std::string &var) const
      throw(SessionException);
  template<class T>
  void Set(const std::string &var, const T &value)
      throw(SessionException);
 private:
  bool running_;
  std::auto_ptr<boost::thread> listen_thread_;
  typedef boost::unordered_map<std::string, contraption::FieldTypeP> SessionMap;
  SessionMap session_;
 protected:
  static log4cplus::Logger logger_;
  RequestProcessorP request_processor_;
  virtual void ListenThread()
      throw() {};
};

}
}
}  
#include "server_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__SERVER_HPP_
