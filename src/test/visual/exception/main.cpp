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

#include <gui_exception/gui_exception.hpp>
#include <wx/xrc/xmlres.h>

using namespace tms::common;

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
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
 public:
  // ctor(s)
    MyFrame(const wxString& title);
 private:
  // any class wishing to process wxWidgets events must use this macro
  DECLARE_EVENT_TABLE()
};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(ExceptionTest)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool ExceptionTest::OnInit()
{
  // call the base class initialization method, currently it only parses a
  // few common command-line options but it could be do more in the future
  if ( !wxApp::OnInit() )
    return false;
  wxImage::AddHandler( new wxPNGHandler );
  wxXmlResource::Get()->InitAllHandlers();
  // create the main application window
  MyFrame *frame = new MyFrame(_T("TestApp"));
  frame->Show(true);
  // Success: wxApp::OnRun() will be called which will enter the main message
  // loop and the application will run. If we returned false here, the
    // application would exit immediately.
  try {
    throw GUIException("Libraty intr.dll was not found");
  } catch (GUIException e) {
    e.Report();
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
}
