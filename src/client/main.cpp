
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
// log4cplus
#include <client/logger.hpp>
// log4cplus
#include <log4cplus/configurator.h>

#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/image.h>

// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// frames
#include <client/frm_grid.hpp>
#include <client/dlg_login.hpp>
// gcc
#ifndef WINDOWS32
#include <execinfo.h>
#endif
using namespace tms::client;
using namespace tms::common::string;
using namespace log4cplus;

class ClientApp : public wxApp {
 public:
  ClientApp() {}
  virtual bool OnInit();
  virtual bool OnExceptionInMainLoop();
  virtual int OnExit();
 private:
  ClientApp(const ClientApp&);
  ClientApp& operator=(const ClientApp&);
};

IMPLEMENT_APP(ClientApp)

bool ClientApp::OnExceptionInMainLoop() {
  const size_t nest = 20;
  try {
    throw;
  } catch (const tms::common::GUIException &e) {
    #ifndef WINDOWS32
    void *array[nest];
    size_t size;
    char **strings;
    size_t i;
    
    size = backtrace (array, nest);
    strings = backtrace_symbols (array, size);
    LOG4CPLUS_ERROR(client_logger,
                    WStringFromUTF8String(e.what()));     
    for (i = 0; i < nest; ++i) {
      LOG4CPLUS_ERROR(client_logger,
                      WStringFromUTF8String(strings[i]));     
    }
    free(strings);
    #endif
    Report(e);
  } catch (std::exception &e) {
    #ifndef WINDOWS32
    void *array[nest];
    size_t size;
    char **strings;
    size_t i;
    
    size = backtrace (array, nest);
    strings = backtrace_symbols (array, size);
    LOG4CPLUS_ERROR(client_logger,
                    WStringFromUTF8String(e.what()));     
    for (i = 0; i < nest; ++i) {
      LOG4CPLUS_ERROR(client_logger,
                      WStringFromUTF8String(strings[i]));     
    }
    free(strings);
    #endif        
    wxMessageDialog(0,
                  WStringFromUTF8String("Произошла критическая ошибка.")
                    ).ShowModal();    
    throw;
  }
  return true;
}

int ClientApp::OnExit() {
  wxXmlResource::Get()->Unload(_T("xrc/client/frm_grid.xrc"));
  wxXmlResource::Get()->Unload(_T("xrc/client/dlg_login.xrc"));
  wxXmlResource::Get()->Unload(_T("xrc/client/dlg_user.xrc"));
  wxXmlResource::Get()->Unload(_T("xrc/client/dlg_contact_person.xrc"));
  wxXmlResource::Get()->Unload(_T("xrc/client/CompaniesFrame.xrc"));
  wxXmlResource::Get()->Unload(_T("xrc/client/IncomingsFrame.xrc"));
  delete (wxXmlResource::Set(0));
  return 0;
}

bool ClientApp::OnInit() {
  PropertyConfigurator config(WStringFromUTF8String("log.cfg"));
  config.configure();
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Client started"));
  bool wxsOK = true;
  wxTheApp->SetExitOnFrameDelete(true);
  try {
    wxXmlResource::Get()->InitAllHandlers();
    wxInitAllImageHandlers();
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/frm_grid.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/dlg_login.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/dlg_user.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/dlg_incoming.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/dlg_company.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T(
        "xrc/client/dlg_contact_person.xrc"));
    if (!wxsOK) {
      LOG4CPLUS_ERROR(client_logger,
                      WStringFromUTF8String("Error while loading xrc resources"));
      return false;
    }
    if (!wxApp::OnInit()) {
      LOG4CPLUS_ERROR(client_logger,
                      WStringFromUTF8String(
                          "Error during application initialization"));     
      return false;
    }
    if (DlgLogin().ShowModal() == wxOK) {
      (new FrmGrid())->Show(true);
    } else {
      OnExit();
      return false;
    }
  } catch (tms::common::GUIException &e) {
    std::cerr << e.message() << std::endl;
  }
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Forms are initialized"));
  return wxsOK;
}
