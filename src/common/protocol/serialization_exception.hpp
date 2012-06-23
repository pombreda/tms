#ifndef _SERIALIZATION_EXCEPTION_HPP_
#define _SERIALIZATION_EXCEPTION_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <gui_exception/gui_exception.hpp>
namespace tms {
namespace common {

class SerializationException : public GUIException {
 public:
  explicit SerializationException(const std::string &message) :
      GUIException(message) {
  }
  
  explicit SerializationException(const std::exception *std_exception, const std::string &message = "") throw ():
      GUIException(std_exception, message) {
  }
  
  SerializationException(const GUIException *gui_exception, const std::string &message) throw ():
      GUIException(gui_exception, message) {
  }
  
  virtual std::string message() {
    return std::string("SerializationException: ") + GUIException::message();
  }
};

}
}
#endif // _SERIALIZATION_EXCEPTION_HPP_
