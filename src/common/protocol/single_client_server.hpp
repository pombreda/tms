#ifndef _TMS_COMMON_PROTOCOL__SINGLE_CLIENT_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SINGLE_CLIENT_SERVER_HPP_

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
namespace tms {
namespace common {
namespace protocol {


class SingleClientServer : public Server {
 public:
  SingleClientServer();
  explicit SingleClientServer(HandlersMapP handlers_map);
 protected:
  virtual MessageP Eval(const Message &message)
      throw(ServerException);
};

typedef boost::shared_ptr<SingleClientServer> SingleClientServerP;
}
}
}  
#endif // _TMS_COMMON_PROTOCOL__STREAM_SERVER_HPP_
