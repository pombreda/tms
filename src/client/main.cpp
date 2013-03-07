
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
// frames
#include <client/dlg_login.hpp>

using namespace tms::client;
using namespace tms::common::string;
using namespace log4cplus;

class ClientApp : public wxApp {
 public:
  ClientApp() {}
  virtual bool OnInit();
 private:
  ClientApp(const ClientApp&);
  ClientApp& operator=(const ClientApp&);
};

IMPLEMENT_APP(ClientApp)

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
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T(
        "xrc/client/dlg_contact_person.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T(
        "xrc/client/dlg_check_column.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/CompaniesFrame.xrc"));
    wxsOK = wxsOK && wxXmlResource::Get()->Load(_T("xrc/client/IncomingsFrame.xrc"));
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
    DlgLogin *login_frame = new DlgLogin();
    login_frame->ShowModal();
  } catch (tms::common::GUIException &e) {
    std::cerr << e.message() << std::endl;
  }
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Forms are initialized"));
  return wxsOK;
}
