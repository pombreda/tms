#ifndef _TMS_COMMON_PROTOCOL__SERVER_EXCEPTION_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_EXCEPTION_HPP__
#include <gui_exception/gui_exception.hpp>
namespace tms {
namespace common {
namespace protocol {

class ServerException : public GUIException {
 public:
  explicit ServerException(const std::string &message) throw () : 
      GUIException(message) { }
  
  explicit ServerException(const std::exception *std_exception, const std::string &message = "") throw () :
      GUIException(std_exception, message) { }   
  
  ServerException(const GUIException *gui_exception, const std::string &message) throw () : 
      GUIException(gui_exception, message) {}

};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__SERVER_EXCEPTION_HPP__
