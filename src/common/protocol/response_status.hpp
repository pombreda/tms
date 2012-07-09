#ifndef _TMS_COMMON_PROTOCOL__RESPONSE_STATUS_HPP_
#define _TMS_COMMON_PROTOCOL__RESPONSE_STATUS_HPP_

namespace tms {
namespace common {
namespace protocol {
//------------------------------------------------------------
// Unique ids for different requests
//------------------------------------------------------------

namespace ResponseStatus {
enum ResponseStatus {
  kOk,
  kAuthenticationError,
  kIncorrectVersion,
  kServerError,
  kIncorrect
};
}

}
}
}
#endif // _TMS_COMMON_PROTOCOL__RESPONSE_STATUS_HPP_
