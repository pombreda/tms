#ifndef _TMS_COMMON_PROTOCOL__MODEL_BACKEND_PROTOCOL_HPP_
#define _TMS_COMMON_PROTOCOL__MODEL_BACKEND_PROTOCOL_HPP_
// boost
#include <boost/intrusive_ptr.hpp>
// common
#include <protocol/login_protocol.hpp>

namespace tms {
namespace common {
namespace protocol {

class ModelBackendProtocol : public LoginProtocol {
 public:
  ModelBackendProtocol()
      throw(ProtocolException);
  ModelBackendProtocol(const Protocol &other)
      throw(ProtocolException);
 private:
  void AddClasses()
      throw(ProtocolException);
};

typedef boost::intrusive_ptr<ModelBackendProtocol> ModelBackendProtocolP;

}
}
}
#endif // _TMS_COMMON_PROTOCOL__MODEL_BACKEND_PROTOCOL_HPP_
