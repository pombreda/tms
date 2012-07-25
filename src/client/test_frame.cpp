#include "test_frame.hpp"

#include <wx/xrc/xmlres.h>

BEGIN_EVENT_TABLE(TestFrame,wxFrame)
END_EVENT_TABLE()

TestFrame::~TestFrame() {
}

void TestFrame::Init() {
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&TestFrame::OnSaveClick);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&TestFrame::OnDeleteClick);
  Connect(XRCID("ID_BUTTON3"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&TestFrame::OnExitClick);
  Bind(wxEVT_CLOSE_WINDOW, &TestFrame::OnTryClose, this);
}

void TestFrame::SetUpValues(ContraptionP contraption, ContraptionArrayP contraptions) {
  contraption_ = contraption;
  contraptions_ = contraptions;
  tc_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  tc_name_->ChangeValue(contraption_->Get<std::string>("name"));
  tc_surname_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  tc_surname_->ChangeValue(contraption_->Get<std::string>("Surname"));
  tc_age_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  tc_age_->ChangeValue("");
  *tc_age_ << contraption_->Get<int>("age");
}

void TestFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Set<std::string>("name", tc_name_->GetValue().ToStdString());
  contraption_->Set<std::string>("Surname", tc_surname_->GetValue().ToStdString());
  contraption_->Set<int>("age", boost::lexical_cast<int>(tc_age_->GetValue().ToStdString()));
  contraption_->Save();
  contraptions_->Refresh();
  Hide();
}

void TestFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  Hide();
}

void TestFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void TestFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  Hide();
}
