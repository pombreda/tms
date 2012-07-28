//-----------------------------------------------------------------------------
// Standard wxWidgets headers
//-----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#define TMS_GUI
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//-----------------------------------------------------------------------------
// Headers of this .cpp file
//-----------------------------------------------------------------------------

#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/image.h>

// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <gui_exception/gui_exception.hpp>
// frames
#include "login_frame.hpp"

using namespace tms::client;

class ClientApp : public wxApp {
 public:
  ClientApp() :
      login_frame() {}
  virtual bool OnInit();

 private:
  ClientApp(const ClientApp&);
  ClientApp& operator=(const ClientApp&);

  LoginFrame *login_frame;
};

IMPLEMENT_APP(ClientApp)

bool ClientApp::OnInit() {
  bool wxsOK = true;
  try {
    wxXmlResource::Get()->InitAllHandlers();
    wxInitAllImageHandlers();
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/GridFrame.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/LoginFrame.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/TestFrame.xrc"));
    if (!wxApp::OnInit())
      return false;
   	login_frame = new LoginFrame();
   	wxXmlResource::Get()->LoadFrame(login_frame, NULL, _T("LoginFrame"));
   	login_frame->Init();
    login_frame->Show(true);
  } catch (tms::common::GUIException &e) {
    std::cerr << e.message() << std::endl;
  }
  return wxsOK;
}
