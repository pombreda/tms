#include "login_frame.hpp"

namespace tms {
namespace client {

BEGIN_EVENT_TABLE(LoginFrame,wxFrame)
END_EVENT_TABLE()

LoginFrame::~LoginFrame() {
}

void LoginFrame::LoadOptions() {
  Options::Init();
  wxTextCtrl *text;
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  text->ChangeValue(Options::name());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  // here should be some processing of password_hash
  text->ChangeValue("*****");
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  text->ChangeValue(Options::server());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  text->ChangeValue(Options::port());
}

void LoginFrame::SaveOptions() {
  wxTextCtrl *text;
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  Options::set_name(text->GetValue().ToStdString());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  // here should be setting password_hash
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  Options::set_server(text->GetValue().ToStdString());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  Options::set_port(text->GetValue().ToStdString());
}

void LoginFrame::Init() {
  Centre();
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&LoginFrame::OnOKButtonClick);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&LoginFrame::OnExitButtonClick);
  LoadOptions();
}

void LoginFrame::OnOKButtonClick(wxCommandEvent& WXUNUSED(event)) {
  SaveOptions();
  grid_frame = new GridFrame();
 	wxXmlResource::Get()->LoadFrame(grid_frame, NULL, _T("GridFrame"));
 	grid_frame->Init();
  grid_frame->SetTitle(_T("Журнал"));
  grid_frame->Show(true);
  Close();
}

void LoginFrame::OnExitButtonClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
