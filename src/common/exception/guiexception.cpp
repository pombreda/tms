#include <exception/guiexception.hpp>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>
using namespace tms::common;
using namespace std;

string GUIException::Message() {
  ostringstream ret;
  if (message_.size() > 0) {
    ret << message_;
    if (std_exception_ || gui_exception_) {
      ret << std::endl;
    }
  }
  if (std_exception_) {
    ret << std_exception_->what();
  }
  if (gui_exception_) {
    ret << gui_exception_->Message();
  }
  return ret.str();
}

void GUIException::Report() {
  fl_input("Error occured:", Message().c_str());
}
