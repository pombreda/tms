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
    wxDialog(),
    logger_(Logger::getInstance(WStringFromUTF8String(rtti::TypeID(this).name()))) {
  wxXmlResource::Get()->Load(_T("xrc/common/widget/dlg_check_column.xrc"));
    
  wxXmlResource::Get()->LoadDialog(this, parent,
                                   _T("dlgCheckColumn"));
  Init();
}

DlgCheckColumn::~DlgCheckColumn() {
}

void DlgCheckColumn::Init() {
  check_list_ = XRCCTRL(*this, "chlColumns", wxCheckListBox);
  check_list_->Connect(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,
                       wxCommandEventHandler( DlgCheckColumn::OnItemChecked ),
                       0, this);
  Connect(wxEVT_ACTIVATE, wxActivateEventHandler(DlgCheckColumn::OnActivate));

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

void DlgCheckColumn::OnActivate(wxActivateEvent &event) {
  if (!event.GetActive()) {
    Close();
  }
}

}
}
}
