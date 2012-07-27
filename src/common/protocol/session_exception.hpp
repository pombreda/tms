#ifndef _TMS_COMMON_PROTOCOL__SESSION_EXCEPTION_HPP_
#define _TMS_COMMON_PROTOCOL__SESSION_EXCEPTION_HPP_
#include <gui_exception/gui_exception.hpp>
namespace tms {
namespace common {
namespace protocol {

class SessionException : public GUIException {
 public:
  explicit SessionException(const std::string &message) throw () : 
      GUIException(message) { }
  
  explicit SessionException(const std::exception *std_exception, const std::string &message = "") throw () :
      GUIException(std_exception, message) { }   
  
  SessionException(const GUIException *gui_exception, const std::string &message) throw () : 
      GUIException(gui_exception, message) {}

};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__SESSION_EXCEPTION_HPP__
