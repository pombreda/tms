#ifndef _TMS_COMMON_PROTOCOL__PROTOCOL_EXCEPTION_HPP_
#define _TMS_COMMON_PROTOCOL__PROTOCOL_EXCEPTION_HPP__
#include <gui_exception/gui_exception.hpp>
namespace tms {
namespace common {
namespace protocol {

class ProtocolException : public GUIException {
 public:
  explicit ProtocolException(const std::string &message) throw () : 
      GUIException(message) { }
  
  explicit ProtocolException(const std::exception *std_exception, const std::string &message = "") throw () :
      GUIException(std_exception, message) { }   
  
  ProtocolException(const GUIException *gui_exception, const std::string &message) throw () : 
      GUIException(gui_exception, message) {}

};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__PROTOCOL_EXCEPTION_HPP__
