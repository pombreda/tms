#ifndef _GUI_EXCEPTION_HPP_
#define _GUI_EXCEPTION_HPP_

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include <string>
#include <exception>
#include <sstream>

namespace tms {
  namespace common {

    //-----------------------------------------------------------------------------
    // Class definition: GuiException
    //-----------------------------------------------------------------------------

    class GUIException {
    public: 
      explicit GUIException(std::string message) :
	message_(message),
	std_exception_(0),
	gui_exception_(0)
      {
      }
      
      explicit GUIException(const std::exception &std_exception, std::string message = "") :
	message_(message),
	std_exception_(new std::exception(std_exception)),
	gui_exception_(0)
      {
      }
      explicit GUIException(const GUIException &gui_exception, std::string message) :
	message_(message),
	std_exception_(0),
	gui_exception_(new GUIException(gui_exception))
      {
      }
      void Report();
      std::string Message();
    private:
      std::string message_;
      std::exception* std_exception_;
      GUIException* gui_exception_;
    };
  }
}
#endif // _GUI_EXCEPTION_HPP_