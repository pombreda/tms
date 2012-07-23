#include "TestFrame.h"

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
  tc_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  tc_name_->ChangeValue("");
  tc_surname_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  tc_surname_->ChangeValue("");
  tc_age_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  tc_age_->ChangeValue("");
}

void TestFrame::SetUpValues(const std::string &name, const std::string &surname,
                            int age) {
  *tc_name_ << name;
  *tc_surname_ << surname;
  *tc_age_ << age;
}

void TestFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {

}

void TestFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {

}

void TestFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}
