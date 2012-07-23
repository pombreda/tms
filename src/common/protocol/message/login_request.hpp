#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__LOGIN_REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__LOGIN_REQUEST_HPP_
#include <protocol/message/login_request.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<LoginRequest> LoginRequestP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__LOGIN_REQUEST_HPP_
