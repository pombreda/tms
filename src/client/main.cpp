// the application icon (under Windows and OS/2 it is in resources)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "icon.xpm"
#endif

// log4cplus
#define TMS_GUI
#include <client/logger.hpp>
// log4cplus
#include <log4cplus/configurator.h>

#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/image.h>
#include <wx/app.h>

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
  virtual void OnFatalException();
  virtual int OnExit();
 private:
  ClientApp(const ClientApp&);
  ClientApp& operator=(const ClientApp&);
};

IMPLEMENT_APP(ClientApp)
static void LogBacktrace() {
#ifndef WINDOWS32
  const size_t nest = 20;
  void *array[nest];
  size_t size;
  char **strings;
  size_t i;
    
  size = backtrace (array, nest);
  strings = backtrace_symbols (array, size);
  for (i = 0; i < nest; ++i) {
    LOG4CPLUS_ERROR(client_logger,
        WStringFromUTF8String(strings[i]));     
  }
  free(strings);
#endif
}

bool ClientApp::OnExceptionInMainLoop() {
  try {
    throw;
  } catch (const tms::common::GUIException &e) {
    LOG4CPLUS_ERROR(client_logger,
                    WStringFromUTF8String(e.what()));     
    LogBacktrace();
    Report(e);
  } catch (std::exception &e) {
    LOG4CPLUS_ERROR(client_logger,
                    WStringFromUTF8String(e.what()));     
    LogBacktrace();
    wxMessageDialog(0,
                    WStringFromUTF8String("Произошла критическая ошибка."),
                    WStringFromUTF8String("Ошибка"),
                    wxOK | wxICON_ERROR
                    ).ShowModal();    
    throw;
  }
  return true;
}

void ClientApp::OnFatalException() {
   wxMessageDialog(0,
                   WStringFromUTF8String("Произошла критическая ошибка."),
                   WStringFromUTF8String("Ошибка"),
                   wxOK | wxICON_ERROR
                   ).ShowModal();    
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
  if (!wxApp::OnInit()) {
    LOG4CPLUS_ERROR(client_logger,
                    WStringFromUTF8String(
                        "Error during application initialization"));     
    return false;
  }

  bool wxsOK = true;
  SetExitOnFrameDelete(true);
  try {
    wxXmlResource::Get()->InitAllHandlers();
    wxInitAllImageHandlers();
    wxsOK = wxsOK && wxXmlResource::Get()->LoadAllFiles(
        WStringFromUTF8String("xrc/client"));
    if (!wxsOK) {
      LOG4CPLUS_ERROR(client_logger,
                      WStringFromUTF8String("Error while loading xrc resources"));
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
