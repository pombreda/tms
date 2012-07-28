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
  tc_name_->ChangeValue(contraption_->Get<std::string>("name"));
  tc_code_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  tc_code_->ChangeValue(contraption_->Get<std::string>("code"));
  tc_email_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  tc_email_->ChangeValue(contraption_->Get<std::string>("email"));
  tc_phone_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  tc_phone_->ChangeValue(contraption_->Get<std::string>("phone"));
  tc_fax_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL5");
  tc_fax_->ChangeValue(contraption_->Get<std::string>("fax"));
  tc_notes_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL6");
  tc_notes_->ChangeValue(contraption_->Get<std::string>("note"));
}

void ContactPersonsFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Set<std::string>("name", tc_name_->GetValue().ToStdString());
  contraption_->Set<std::string>("code", tc_code_->GetValue().ToStdString());
  contraption_->Set<std::string>("email", tc_email_->GetValue().ToStdString());
  contraption_->Set<std::string>("phone", tc_phone_->GetValue().ToStdString());
  contraption_->Set<std::string>("fax", tc_fax_->GetValue().ToStdString());
  contraption_->Set<std::string>("note", tc_notes_->GetValue().ToStdString());
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
