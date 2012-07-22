#ifndef _TMS_TEST_UNIT_COMMON_PROTOCOL__DUMMY_PROTOCOL_
#define _TMS_TEST_UNIT_COMMON_PROTOCOL__DUMMY_PROTOCOL_
#include <protocol/protocol.hpp>
#include "dummy.pb.h"
namespace tms {
namespace test {
class DummyProtocol : public tms::common::protocol::Protocol {
 public:
  DummyProtocol() : Protocol() {
    AddMessageClass<DummyMessage>();
    AddMessageClass<DummyMessage2>();
    Initialize();
  }
};

}
}
#endif // _TMS_TEST_UNIT_COMMON_PROTOCOL__DUMMY_PROTOCOL_
