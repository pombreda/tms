#ifndef _GUI_EXCEPTION_HPP_
#define _GUI_EXCEPTION_HPP_

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include <string>
#include <exception>
#include <sstream>
#include <memory>

#ifdef TMS_GUI
#include <gui_exception/gui_exception_report_dialog.hpp>
#else
#include <iostream>
#endif


namespace tms {
namespace common {

//-----------------------------------------------------------------------------
// Class definition: GuiException
//-----------------------------------------------------------------------------

class GUIException : public std::exception {
 public: 
  explicit GUIException(std::string message) {
    Init(message, "");
  }
  
  explicit GUIException(const std::exception *std_exception, std::string message = "") throw () {
    Init(message, std_exception->what());
  }   
  
  GUIException(const GUIException *gui_exception, std::string message) throw () {
    Init(message, gui_exception->message());
  }
   
  virtual const char* what() const throw () {
    return "GUIException. To obtain full exception message use ::Message()";
  }

  virtual void Report() const {
#ifdef TMS_GUI
    GuiExceptionReportDialog dlg(message());
    dlg.ShowModal();
#else
    std::cerr << message() << std::endl;
#endif
  }

  virtual std::string message() const throw ()
  {
    return message_;
  }
  virtual ~GUIException() throw () {}
 protected:
  std::string message_;
 private:
  void Init(std::string message, std::string prev_message);
};
}
}
#endif // _GUI_EXCEPTION_HPP_
