
#include "contact_persons_frame.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
// boost
#include <boost/lexical_cast.hpp>

namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::string;

BEGIN_EVENT_TABLE(ContactPersonsFrame,wxFrame)
END_EVENT_TABLE()

ContactPersonsFrame::~ContactPersonsFrame() {
}

void ContactPersonsFrame::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing ContactPersonsFrame"));

  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&ContactPersonsFrame::OnSaveClick);

  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&ContactPersonsFrame::OnDeleteClick);


  Connect(XRCID("ID_BUTTON3"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&ContactPersonsFrame::OnExitClick);

  Bind(wxEVT_CLOSE_WINDOW, &ContactPersonsFrame::OnTryClose, this);

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("ContactPersonsFrame initialized"));
  
}

void ContactPersonsFrame::SetUpValues(ContraptionP contraption,
				      ContraptionArrayP contraptions, bool editable) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting contact person values"));
  tc_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  tc_code_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  tc_email_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  tc_phone_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  tc_fax_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL5");
  tc_notes_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL6");
  tc_role_ = (wxTextCtrl*)FindWindowByName("ID_TEXTROLE", this);
  tc_company_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCOMPANY", this);
  button_remove_ = (wxButton*)FindWindowByName("ID_BUTTON2", this);
  button_save_ = (wxButton*)FindWindowByName("ID_BUTTON1", this);

  contraption_ = contraption;
  contraptions_ = contraptions;
  button_remove_->Show(true);
  button_save_->Show(true);
  if (contraption->IsNew()) {
    button_remove_->Show(false);
  } 
  if (!editable) {
    button_remove_->Show(false);
    button_save_->Show(false);
  }
  std::string name = contraption_->Get<std::string>("name");
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("Name is: " + name));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String(boost::lexical_cast<std::string>((int)tc_name_)));
  tc_name_->ChangeValue(wxString::FromUTF8(name.c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("name set"));
  tc_code_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("code").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("code set"));

  tc_email_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("email").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("email set"));

  tc_role_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("role").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("role set"));

  tc_company_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("company").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("company set"));

  tc_phone_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("phone").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("phone set"));

  tc_fax_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("fax").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("fax set"));

  tc_notes_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("note").c_str()));
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("note set"));

  Fit();
  Layout();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Contact person values set"));
}

void ContactPersonsFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Set<std::string>("name", tc_name_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("code", tc_code_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("email", tc_email_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("phone", tc_phone_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("fax", tc_fax_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("note", tc_notes_->GetValue().utf8_str().data());
  contraption_->Save();
  contraptions_->Refresh();
  Hide();
}

void ContactPersonsFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  Hide();
}

void ContactPersonsFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void ContactPersonsFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  Hide();
}

}
}
