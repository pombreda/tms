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

// std
#include <cstdio>
#include <iostream> //oops
// boost
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/contraption_array.hpp>
#include <widget/contraption_grid.hpp>
#include <gui_exception/gui_exception.hpp>

using namespace std;
using namespace tms::common::contraption;
using namespace boost;
using namespace tms::common::widget;
using namespace tms::common;

void OnClick(ContraptionP &contraption, FieldID field_id) {
  contraption->Set<string>("name", string("Ivan"));
}

static void init(ModelBackendP &backend, ModelP &model) {
  string test_db("test.sqlite3");
  remove(test_db.c_str());
  SOCIDBScheme scheme(soci::sqlite3, test_db);
  backend.reset(new SOCIModelBackend(scheme, "test"));
  vector<Field*> fields;
  fields.push_back(new SimpleFieldT<string>("name"));
  fields.push_back(new SimpleFieldT<int>("age"));
  fields.push_back(new SimpleFieldT<int>("password",
                                         _is_readable = false));
  fields.push_back(new SimpleFieldT<string>("Surname",
                                            _backend_name = "surname"));
  model.reset(new Model(fields, backend));
  model->InitSchema();
  ContraptionP test_contraption;
  for (int i = 0; i < 30; i++) {
    test_contraption = model->New();
    test_contraption->Set<string>("name", string("John") + lexical_cast<string>(i % 100));
    test_contraption->Set<int>("age", i % 40 + 10);
    test_contraption->Set<string>("Surname", string("Smith") + lexical_cast<string>(i % 100));
    test_contraption->Save();
    if ((i + 1) % 10 == 0)
      std::cout << i + 1 << " records are written." << std::endl;
  }
  std::cout << "All records are written!" << std::endl;
}

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
  MyFrame(const wxString& title);
 private:
  ContraptionGrid *grid;
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
bool ExceptionTest::OnInit() {
  // call the base class initialization method, currently it only parses a
  // few common command-line options but it could be do more in the future
  try {
    if ( !wxApp::OnInit() )
      return false;
    wxImage::AddHandler( new wxPNGHandler );
    wxXmlResource::Get()->InitAllHandlers();
    // create the main application window
    MyFrame *frame = new MyFrame(_T("Grid test"));
    frame->Show(true);
  } catch (GUIException &e) {
    std::cerr << e.message() << std::endl;
  }
  // Success: wxApp::OnRun() will be called which will enter the main message
  // loop and the application will run. If we returned false here, the
  // application would exit immediately.
  //  return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title) {

  ModelBackendP backend;
  ModelP model;
  init(backend, model);
  ContraptionArrayP contraptions = model->All();
  vector<Column> cols;
  cols.push_back(Column(0, "Name", 70));
  cols.push_back(Column(3, "Surname", 100));
  cols.push_back(Column(1, "Age", 50));
  grid = new ContraptionGrid(contraptions, cols, this,
                             wxID_ANY, wxPoint(0, 0), wxSize(400, 300));
  function<void(ContraptionP, FieldID)> f = bind(&OnClick, _1, _2);
  grid->SetOnCellClick(f);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(grid, 1, wxEXPAND);
  SetAutoLayout(true);
  SetSizer(topSizer);
  topSizer->Fit(this);
  Centre();
}
