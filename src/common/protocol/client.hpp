#ifndef _TMS_COMMON_PROTOCOL__CLIENT_HPP_
#define _TMS_COMMON_PROTOCOL__CLIENT_HPP_
// std
#include <istream>
// boost 
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>

namespace tms {
namespace common {
namespace protocol {

class Client {
 public:
  virtual MessageP EvalRequest(Message &message) 
      throw (ProtocolException) = 0;
  virtual ~Client() 
      throw(){}
};


typedef boost::shared_ptr<Client> ClientP;
}
}
}
#endif // _TMS_COMMON_PROTOCOL__CLIENT_HPP_
