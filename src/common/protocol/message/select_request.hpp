#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__SELECT_REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__SELECT_REQUEST_HPP_
#include <protocol/message/select_request.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<SelectRequest> SelectRequestP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__SELECT_REQUEST_HPP_
