#include "LoginFrame.h"

#include <wx/xrc/xmlres.h>

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
 	GridFrame *grid_frame = (GridFrame *)wxXmlResource::Get()->LoadFrame(NULL, _T("GridFrame"));
 	grid_frame->Init();
  grid_frame->SetTitle("Журнал");
  grid_frame->Show(true);
  Close();
}

void LoginFrame::OnExitButtonClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}
