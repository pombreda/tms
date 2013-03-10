#include "dlg_check_column.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>
// log4cplus
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <rtti/typeinfo.hpp>
#include <widget/contraption_grid.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// boost
#include <boost/bind.hpp>

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace common {
namespace widget {

using namespace tms::common::string;
using namespace log4cplus;

DlgCheckColumn::DlgCheckColumn(wxWindow *parent) :
    wxPopupTransientWindow(parent),
    logger_(Logger::getInstance(WStringFromUTF8String(rtti::TypeID(this).name()))) {
  panel_ = new wxPanel();
  wxXmlResource::Get()->Load(_T("xrc/common/widget/dlg_check_column.xrc"));
  
  wxXmlResource::Get()->LoadPanel(panel_, this,
                                  _T("pnlCheckColumn"));
  Init();
}

DlgCheckColumn::~DlgCheckColumn() {
}

void DlgCheckColumn::Init() {
  check_list_ = XRCCTRL(*panel_, "chlColumns", wxCheckListBox);
  check_list_->Connect(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,
                       wxCommandEventHandler( DlgCheckColumn::OnItemChecked ),
                       0, this);
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("DlgCheckColumn initalized"));
}

void DlgCheckColumn::SetUpValues(ContraptionGrid *grid) {
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Setting up values"));
  grid_ = grid;
  int col_number = grid_->GetNumberCols();
  check_list_->Clear();
  for (int pos = 0; pos < col_number; ++pos) {
    check_list_->Append(grid_->GetColLabelValue(pos));
    check_list_->Check(pos, grid_->GetColSize(pos) != 0);
  }
  panel_->Layout();
  panel_->Fit();
  Layout();
  Fit();
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Values set up"));
}

void DlgCheckColumn::OnItemChecked(wxCommandEvent& event) {
  int col =  event.GetInt();
  bool state = check_list_->IsChecked(col);
  std::string col_name = grid_->GetColLabelValue(col).ToStdString();
  if (!state) {
    grid_->ShowCol(col, false);
  } else {
    grid_->ShowCol(col, true);
  }
}

}
}
}
