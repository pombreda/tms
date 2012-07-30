c c#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__PATCH_SERVER_REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__PATCH_SERVER_REQUEST_HPP_
#include <protocol/message/patch_server_request.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<PatchServerRequest> PatchServerRequestP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__PATCH_SERVER_REQUEST_HPP_
