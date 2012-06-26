#ifndef _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_EXCEPTION_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_EXCEPTION_HPP_

#include <gui_exception/gui_exception.hpp>

namespace tms {
namespace common {
namespace contraption {

class ModelBackendException : public GUIException {
 public:
  explicit ModelBackendException(const std::string &message) throw () : 
      GUIException(message) { }
  
  explicit ModelBackendException(const std::exception *std_exception, const std::string &message = "") throw () :
      GUIException(std_exception, message) { }   
  
  ModelBackendException(const GUIException *gui_exception, const std::string &message) throw () : 
      GUIException(gui_exception, message) {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_EXCEPTION_HPP_
