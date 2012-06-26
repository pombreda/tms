#ifndef _TMS_COMMON_CONTRAPTION__FIELD_EXCEPTION_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_EXCEPTION_HPP_
#include <gui_exception/gui_exception.hpp>
namespace tms {
namespace common {
namespace contraption {

class FieldException : public GUIException {
 public:
  explicit FieldException(const std::string &message) throw () : 
      GUIException(message) { }
  
  explicit FieldException(const std::exception *std_exception, const std::string &message = "") throw () :
      GUIException(std_exception, message) { }   
  
  FieldException(const GUIException *gui_exception, const std::string &message) throw () : 
      GUIException(gui_exception, message) {}

};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_EXCEPTION_HPP_
