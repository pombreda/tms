#ifndef _TMS_COMMON_PROTOCOL__REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL__REQUEST_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <string>
#include <protocol/crypto.hpp>
#include <setup.hpp>
#include <istream>
#include <ostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>

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

class Request {
 public:
  Request() {}
  
  virtual ~Request(){}
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__REQUEST_HPP_
