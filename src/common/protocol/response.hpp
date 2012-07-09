#ifndef _TMS_COMMON_PROTOCOL__RESPONSE_HPP_
#define _TMS_COMMON_PROTOCOL__RESPONSE_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <string>
#include <protocol/crypto.hpp>
#include <protocol/serialization_id.hpp>
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
// Class definition: Request
// Base class for all server requests
//------------------------------------------------------------

class Response {
 public:
  Response() { }

  virtual ~Response(){}

};

}
}
}

#endif // _TMS_COMMON_PROTOCOL__RESPONSE_HPP_
