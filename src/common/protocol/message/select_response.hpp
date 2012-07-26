#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__SELECT_RESPONSE_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__SELECT_RESPONSE_HPP_
#include <protocol/message/select_response.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<SelectResponse> SelectResponseP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__SELECT_RESPONSE_HPP_
