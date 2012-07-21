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

#include <cstdio>
#include <iostream>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <widget/printer.hpp>
#include <gui_exception/gui_exception.hpp>

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace tms::common::contraption;
using namespace boost;
using namespace tms::common::widget;
using namespace tms::common;

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
  ContraptionP test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Du'\"\\mmy");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 12);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
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
  wxGrid *grid;
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
    : wxFrame(NULL, wxID_ANY, title)
{

  ModelBackendP backend;
  ModelP model;
  init(backend, model);
  ContraptionArrayP contraptions = model->All();
  int k = model->GetFieldNumber();
  int n = contraptions->size();

  grid = new wxGrid(this, wxID_ANY, wxPoint(0, 0), wxSize(400, 300));
  grid->CreateGrid(0, 0);
  grid->AppendRows(n);
  grid->AppendCols(k);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(grid, 1, wxEXPAND);
  SetAutoLayout(true);
  SetSizer(topSizer);
  topSizer->Fit(this);
  Centre();

  Printer* printer[k];
  for (int i = 0; i < k; i++) {
    if (dynamic_cast<const FieldT<int>*>(model->GetField(i)) != 0) {
      printer[i] = new PrinterT<int>();
    } else if (dynamic_cast<const FieldT<string>*>(model->GetField(i)) != 0) {
      printer[i] = new PrinterT<string>();
    }
    string name = model->GetField(i)->name();
    grid->SetColLabelValue(i, _T(name));
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < k; j++) {
      if (model->GetField(j)->IsReadable()) {
        grid->SetCellValue(i, j, _T(printer[j]->ToString(*(contraptions->at(i)->GetFieldValue(j)))));
      }
    }
  }
}
