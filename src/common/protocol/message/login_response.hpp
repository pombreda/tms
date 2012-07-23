#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__LOGIN_RESPONSE_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__LOGIN_RESPONSE_HPP_
#include <protocol/message/login_response.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<LoginResponse> LoginResponseP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__LOGIN_RESPONSE_HPP_
