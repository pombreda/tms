#include "dlg_user.hpp"
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
// common
#include <protocol/crypto.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/bool_validator.hpp>
#include <widget/validators/password_validator.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

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

DlgUser::DlgUser(wxWindow *parent) :
    wxDialog(), contraption_(), contraptions_() {
  wxXmlResource::Get()->LoadDialog(this, parent,
                                   _T("dlgUser"));
  Init();
}

void DlgUser::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgUsers"));

  XRCCTRL(*this, "txtName", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "name"),
                      ContraptionSetter<std::string>(contraption_, "name")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Name binded"));
  
  XRCCTRL(*this, "txtPassword", wxTextCtrl)->SetValidator(
      PasswordValidator(ContraptionSetter<std::string>(contraption_, "password_hash")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Password binded"));
    
  XRCCTRL(*this, "chkAdmin", wxCheckBox)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "admin"),
                    ContraptionSetter<bool>(contraption_, "admin")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Admin binded"));
  
  XRCCTRL(*this, "chkSecretary", wxCheckBox)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "secretary"),
                    ContraptionSetter<bool>(contraption_, "secretary")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Secretary binded"));
  
  XRCCTRL(*this, "chkManager", wxCheckBox)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "manager"),
                    ContraptionSetter<bool>(contraption_, "manager")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Manager binded"));

  XRCCTRL(*this, "btnSave", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                               (wxObjectEventFunction)&DlgUser::OnSaveClick,
                                               0, this);
  button_delete_ = XRCCTRL(*this, "btnDelete", wxButton);
  button_delete_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                          (wxObjectEventFunction)&DlgUser::OnDeleteClick,
                          0, this);
  XRCCTRL(*this, "btnCancel", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                               (wxObjectEventFunction)&DlgUser::OnExitClick,
                                               0, this);
  
  Bind(wxEVT_CLOSE_WINDOW, &DlgUser::OnTryClose, this);
  
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgUser Initialized"));
}

void DlgUser::SetUpValues(ContraptionP contraption,
                            ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting up DlgUser values"));
  contraption_ = contraption;

  contraptions_ = contraptions;
  
  if (contraption->IsNew()) {
    button_delete_->Show(false);
  } else {
    button_delete_->Show(true);
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
  } catch (GUIException &e) {
    Report(e);
  }
			     
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
  EndModal(wxOK);
}

void DlgUser::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  FramesCollection::frm_grid->Refresh();

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("User deleted"));
  EndModal(wxOK);
}

void DlgUser::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Closed"));
  EndModal(wxOK);
}

void DlgUser::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Closed"));
  EndModal(wxOK);
}

}
}
