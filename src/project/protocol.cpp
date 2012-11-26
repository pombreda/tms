#include "protocol.hpp"
// common
#include <protocol/patcher_protocol.hpp>
using namespace tms::project;
using namespace tms::common::protocol;

ProtocolP tms::project::AppProtocol() {
  ProtocolP protocol(new PatcherProtocol());
  protocol->Initialize();
  return protocol;
}
