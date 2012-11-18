#ifndef TMS_COMMON_GUI_EXCEPTION__GUI_EXCEPTION_REPORT_HPP
#define TMS_COMMON_GUI_EXCEPTION__GUI_EXCEPTION_REPORT_HPP
#include <gui_exception/gui_exception_report_dialog.hpp>
#include <iostream>

namespace tms {
namespace common {

inline void Report(const GUIException &e) {
  GuiExceptionReportDialog dlg(e.message());
  dlg.ShowModal();
}

}
}
#endif // TMS_COMMON_GUI_EXCEPTION__GUI_EXCEPTION_REPORT_HPP
