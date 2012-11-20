#ifndef _TMS_COMMON_PROTOCOL__PATCHER_PROTOCOL_HPP_
#define _TMS_COMMON_PROTOCOL__PATCHER_PROTOCOL_HPP_
// boost
#include <boost/shared_ptr.hpp>
// common
#include <protocol/model_backend_protocol.hpp>

namespace tms {
namespace common {
namespace protocol {

class PatcherProtocol : public ModelBackendProtocol {
 public:
  PatcherProtocol()
      throw(ProtocolException);
  PatcherProtocol(const Protocol &other)
      throw(ProtocolException);
 private:
  void AddClasses()
      throw(ProtocolException);
};

typedef boost::shared_ptr<PatcherProtocol> PatcherProtocolP;

}
}
}
#endif // _TMS_COMMON_PROTOCOL__PATCHER_PROTOCOL_HPP_
