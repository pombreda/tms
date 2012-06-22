#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report_dialog.hpp>
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
  GuiExceptionReportDialog dlg(Message());
  dlg.ShowModal();
}
