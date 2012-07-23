#ifndef _TMS_COMMON_PROTOCOL__LOGIN_PROTOCOL_HPP_
#define _TMS_COMMON_PROTOCOL__LOGIN_PROTOCOL_HPP_
// boost
#include <boost/shared_ptr.hpp>
// common
#include <protocol/protocol.hpp>

namespace tms {
namespace common {
namespace protocol {

class LoginProtocol : public Protocol {
 public:
  LoginProtocol()
      throw(ProtocolException);
  LoginProtocol(const Protocol &other)
      throw(ProtocolException);
 private:
  void AddClasses()
      throw(ProtocolException);
};

typedef boost::shared_ptr<LoginProtocol> LoginProtocolP;

}
}
}
#endif // _TMS_COMMON_PROTOCOL__LOGIN_PROTOCOL_HPP_
