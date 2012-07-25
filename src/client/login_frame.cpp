#include "login_frame.hpp"

namespace tms {
namespace client {

BEGIN_EVENT_TABLE(LoginFrame,wxFrame)
END_EVENT_TABLE()

LoginFrame::~LoginFrame() {
}

void LoginFrame::Init() {
  Centre();
  Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LoginFrame::OnOKButtonClick);
  Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LoginFrame::OnExitButtonClick);
}

void LoginFrame::OnOKButtonClick(wxCommandEvent& WXUNUSED(event)) {
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
