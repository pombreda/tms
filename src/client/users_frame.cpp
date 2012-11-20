#include "users_frame.hpp"
// wx
#include <wx/msgdlg.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// common
#include <protocol/crypto.hpp>
namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;
BEGIN_EVENT_TABLE(UsersFrame,wxFrame)
END_EVENT_TABLE()

UsersFrame::~UsersFrame() {
}

void UsersFrame::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing UsersFrame"));
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&UsersFrame::OnSaveClick);
  Connect(XRCID("ID_BUTTONREMOVE"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&UsersFrame::OnDeleteClick);
  Connect(XRCID("ID_BUTTONCANCEL"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&UsersFrame::OnExitClick);
  Bind(wxEVT_CLOSE_WINDOW, &UsersFrame::OnTryClose, this);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("UsersFrame Initialized"));
}

void UsersFrame::SetUpValues(ContraptionP contraption,
                            ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting up UsersFrame values"));
  contraption_ = contraption;
  contraptions_ = contraptions;
  tc_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTNAME");
  tc_name_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("name").c_str()));
  tc_password_ = (wxTextCtrl*)FindWindowByName("ID_TEXTPASSWORD");
  tc_password_->ChangeValue(wxString::FromUTF8(""));
  cb_admin_ = (wxCheckBox*)FindWindowByName("ID_CHECKBOXADMIN");
  cb_admin_->SetValue(contraption_->Get<int>("admin"));
  cb_secretair_ = (wxCheckBox*)FindWindowByName("ID_CHECKBOXSECRETAIR");
  cb_secretair_->SetValue(contraption_->Get<int>("secretair"));
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
}

void UsersFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving values"));
  std::string name = tc_name_->GetValue().utf8_str().data();
  if (name == "") {
    LOG4CPLUS_INFO(client_logger, 
		   WStringFromUTF8String("Unsupported password"));
    wxMessageDialog *msg = new wxMessageDialog(this, wxString::FromUTF8("Попытка создать учетную запись с пустым именем"), wxString::FromUTF8("Ошибка"));
    msg->ShowModal();
    return;   
  }
  contraption_->Set<std::string>("name", name);
  std::string password = tc_password_->GetValue().utf8_str().data();
  try {   
  if (password == "") {
    if (contraption_->IsNew()) {
      LOG4CPLUS_INFO(client_logger, 
		     WStringFromUTF8String("Unsupported password"));
      wxMessageDialog *msg = new wxMessageDialog(this, wxString::FromUTF8("Попытка создать учетную запись с пустым паролем"), wxString::FromUTF8("Ошибка"));
      msg->ShowModal();
      return;
    }
  } else {
    contraption_->Set<std::string>("password_hash", sha256(password));
  }
  contraption_->Set<int>("admin", cb_admin_->GetValue());
  contraption_->Set<int>("secretair", cb_secretair_->GetValue());
  contraption_->Save();
  contraptions_->Refresh();
  } catch (GUIException &e) {
    Report(e);
  }
			     
  Hide();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
}

void UsersFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  Hide();
}

void UsersFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void UsersFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  Hide();
}

}
}
