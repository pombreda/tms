#include "contact_persons_frame.hpp"

namespace tms {
namespace client {

using namespace tms::common::contraption;

BEGIN_EVENT_TABLE(ContactPersonsFrame,wxFrame)
END_EVENT_TABLE()

ContactPersonsFrame::~ContactPersonsFrame() {
}

void ContactPersonsFrame::Init() {
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&ContactPersonsFrame::OnSaveClick);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&ContactPersonsFrame::OnDeleteClick);
  Connect(XRCID("ID_BUTTON3"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&ContactPersonsFrame::OnExitClick);
  Bind(wxEVT_CLOSE_WINDOW, &ContactPersonsFrame::OnTryClose, this);
}

void ContactPersonsFrame::SetUpValues(ContraptionP contraption,
                            ContraptionArrayP contraptions) {
  contraption_ = contraption;
  contraptions_ = contraptions;
  tc_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  tc_name_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("name").c_str()));
  tc_code_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  tc_code_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("code").c_str()));
  tc_email_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  tc_email_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("email").c_str()));
  tc_phone_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  tc_phone_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("phone").c_str()));
  tc_fax_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL5");
  tc_fax_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("fax").c_str()));
  tc_notes_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL6");
  tc_notes_->ChangeValue(wxString::FromUTF8(
      contraption_->Get<std::string>("note").c_str()));
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
