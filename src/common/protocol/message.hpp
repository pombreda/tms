#ifndef _TMS_COMMON_PROTOCOL__REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL__REQUEST_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <string>
#include <istream>
#include <ostream>
// boost
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>
#include <boost/shared_ptr.hpp>
// protobuf
#include <google/protobuf/message_lite.h>

namespace tms {
namespace common {
namespace protocol {

//------------------------------------------------------------
// Unique ids for different requests
//------------------------------------------------------------


//------------------------------------------------------------
// Class definition: Request
// Base class for all server requests
//------------------------------------------------------------
typedef ::google::protobuf::MessageLite Message;

typedef boost::shared_ptr<Message> MessageP;

}
}
}
#endif // _TMS_COMMON_PROTOCOL__REQUEST_HPP_
