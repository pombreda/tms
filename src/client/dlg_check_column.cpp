#include "dlg_check_column.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// boost
#include <boost/bind.hpp>

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace tms::common::string;

BEGIN_EVENT_TABLE(DlgCheckColumn, wxDialog)
END_EVENT_TABLE()

DlgCheckColumn::DlgCheckColumn(wxWindow *parent) {
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

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgCheckColumn initalized"));
}

void DlgCheckColumn::SetUpValues(wxGrid *grid,
                                 LayoutGetter layout_getter,
                                 LayoutSetter layout_setter) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting up values"));
  grid_ = grid;
  layout_setter_ = layout_setter;
  layout_getter_ = layout_getter;
  int col_number = grid_->GetNumberCols();
  LOG4CPLUS_DEBUG(client_logger, 
                 WStringFromUTF8String("before for"));
  check_list_->Clear();
  for (int pos = 0; pos < col_number; ++pos) {
    check_list_->Append(grid_->GetColLabelValue(pos));
    check_list_->Check(pos, grid_->GetColSize(pos) != 0);
  }
  Layout();
  Fit();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
}

void DlgCheckColumn::OnItemChecked(wxCommandEvent& event) {
  int col =  event.GetInt();
  bool state = check_list_->IsChecked(col);
  std::string col_name = grid_->GetColLabelValue(col).ToStdString();
  ColumnLayout layout = layout_getter_(col_name);
  if (!state) {
    layout.enabled = false;
    layout.width = grid_->GetColSize(col);
    grid_->SetColSize(col, 0);
  } else {
    layout.enabled = true;
    grid_->SetColSize(col, layout.width);
  }
  layout_setter_(col_name, layout);
}

void DlgCheckColumn::OnActivate(wxActivateEvent &event) {
  if (!event.GetActive()) {
    Close();
  }
}

}
}
