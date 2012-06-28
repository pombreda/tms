#ifdef TMS_GUI
#include <gui_exception/gui_exception_report_dialog.hpp>
#else
#include <iostream>
#endif

namespace tms {
namespace common {
void Report(const GUIException &e) const {
#ifdef TMS_GUI
  GuiExceptionReportDialog dlg(e.message());
  dlg.ShowModal();
#else
  std::cerr << e.message() << std::endl;
#endif
}
}
}
