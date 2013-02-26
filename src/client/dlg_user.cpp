#include "dlg_user.hpp"
// wx
#include <wx/msgdlg.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// boost
#include <boost/bind.hpp>
// common
#include <protocol/crypto.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/bool_validator.hpp>
#include <widget/validators/password_validator.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"

namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget::validators;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;
BEGIN_EVENT_TABLE(DlgUser,wxDialog)
END_EVENT_TABLE()

DlgUser::~DlgUser() {
}

DlgUser::DlgUser() :
    wxDialog(), contraption_(), contraptions_() {
  Init();
}

void DlgUser::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgUsers"));
  wxXmlResource::Get()->LoadDialog(this, FramesCollection::frm_grid,
                                   _T("dlgUser"));

  Connect(XRCID("btnSave"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&DlgUser::OnSaveClick);
  Connect(XRCID("btnDelete"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&DlgUser::OnDeleteClick);
  Connect(XRCID("btnCancel"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&DlgUser::OnExitClick);
  Bind(wxEVT_CLOSE_WINDOW, &DlgUser::OnTryClose, this);
  FindWindowByName("txtName", this)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "name"),
                      ContraptionSetter<std::string>(contraption_, "name")));
  FindWindowByName("txtPassword", this)->SetValidator(
      PasswordValidator(ContraptionSetter<std::string>(contraption_, "password_hash")));
  FindWindowByName("chkAdmin", this)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "admin"),
                    ContraptionSetter<bool>(contraption_, "admin")));
  FindWindowByName("chkSecretary", this)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "secretair"),
                    ContraptionSetter<bool>(contraption_, "secretair")));
  FindWindowByName("chkManager", this)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "manager"),
                    ContraptionSetter<bool>(contraption_, "manager")));

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgUser Initialized"));
}

void DlgUser::SetUpValues(ContraptionP contraption,
                            ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting up DlgUser values"));
  contraption_ = contraption;

  contraptions_ = contraptions;
  button_remove_ = (wxButton*)FindWindowByName("btnDelete", this);

  if (contraption->IsNew()) {
    button_remove_->Show(false);
  } else {
    button_remove_->Show(true);
  }


  TransferDataToWindow();

  Fit();
  Layout();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
}

void DlgUser::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving values"));
  try {
    if (!Validate() || !TransferDataFromWindow()) {
      return;
    }
    contraption_->Save();
    contraptions_->Refresh();
    FramesCollection::frm_grid->Refresh();
  } catch (GUIException &e) {
    Report(e);
  }
			     
  Hide();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
}

void DlgUser::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  FramesCollection::frm_grid->Refresh();
  Hide();
}

void DlgUser::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void DlgUser::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  Hide();
}

}
}
