#include "LoginFrame.h"

//(*InternalHeaders(LoginFrame)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(LoginFrame)
//*)

BEGIN_EVENT_TABLE(LoginFrame,wxFrame)
	//(*EventTable(LoginFrame)
	//*)
END_EVENT_TABLE()

LoginFrame::~LoginFrame()
{
	//(*Destroy(LoginFrame)
	//*)
}

void LoginFrame::Init() {
  Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LoginFrame::OnOKButtonClick);
  Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LoginFrame::OnExitButtonClick);
}

void LoginFrame::OnOKButtonClick(wxCommandEvent& event)
{
  // smth like open new frame with grid
  wxXmlResource::Get()->Load(_T("xrc/client/GridFrame.xrc"));
 	GridFrame *grid_frame = (GridFrame *)wxXmlResource::Get()->LoadFrame(NULL, _T("GridFrame"));
 	grid_frame->Init();
  grid_frame->Show(true);
}

void LoginFrame::OnExitButtonClick(wxCommandEvent& event)
{
  Close();
}
