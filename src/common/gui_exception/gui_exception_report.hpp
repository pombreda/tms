#include <gui_exception/gui_exception_report_dialog.hpp>
#include <iostream>

namespace tms {
namespace common {

void Report(const GUIException &e) {
  GuiExceptionReportDialog dlg(e.message());
  dlg.ShowModal();
}

}
}
