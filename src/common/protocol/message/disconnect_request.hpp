#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__DISCONNECT_REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__DISCONNECT_REQUEST_HPP_
#include <protocol/message/disconnect_request.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<DisconnectRequest> DisconnectRequestP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__DISCONNECT_REQUEST_HPP_
