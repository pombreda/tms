#ifndef _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
#define _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
// std
#include <istream>
#include <ostream>
// common
#include <protocol/response.hpp>
#include <protocol/request.hpp>
#include <protocol/protocol_exception.hpp>

namespace tms {
namespace common {
namespace protocol {

class Protocol {
 public:
  template<class Transaction>
  AddTransaction(Transaction transaction);
  void Initialize();
  bool IsInitialized();
  // for server
  ResponseP ReadResponse(std::istream &sin)
      throw(ProtocolError);
  ReaquestP ReadRequest(std::istream &sin)
      throw(ProtocolError);
  void WriteRequest(std::ostream &out, 
                    Request &request)
      throw(ProtocolError);
  void WriteRequest(std::ostream &out,
                    Request &request)
      throw(ProtocolError);
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__PROTOCOL_HPP_
