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

// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <gui_exception/gui_exception.hpp>
// frames
#include "login_frame.hpp"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class ExceptionTest : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    ExceptionTest()
      : login_frame() {}
    virtual bool OnInit();

 private:
  LoginFrame *login_frame;
};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
//(*AppHeaders
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
//*)

IMPLEMENT_APP(ExceptionTest)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

bool ExceptionTest::OnInit() {
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
