#include "protocol.hpp"
// common
#include <protocol/model_backend_protocol.hpp>
using namespace tms::project;
using namespace tms::common::protocol;

ProtocolP tms::project::AppProtocol() {
  ProtocolP protocol(new ModelBackendProtocol());
  protocol->Initialize();
  return protocol;
}
