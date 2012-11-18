#include "login_frame.hpp"
// log4cplus
#include <client/logger.hpp>
// log4cplus
#include <log4cplus/configurator.h>

// common
#include <protocol/client.hpp>
#include <protocol/crypto.hpp>
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <contraption/model_backend/server_model_backend.hpp>
#include <contraption/model.hpp>
#include <string/string.hpp>
#include <widget/contraption_grid_table_base.hpp>
// client
#include <client/client.hpp>
// project
#include <project/model/contact_person.hpp>
// wx
#include <wx/msgdlg.h>
#include <wx/grid.h>

namespace tms {
namespace client {

using namespace tms::common::protocol;
using namespace tms::common::contraption;
using namespace tms::common::protocol::message;
using namespace tms::project::model;
using namespace tms::common::widget;
using namespace tms::common::string;

BEGIN_EVENT_TABLE(LoginFrame,wxFrame)
END_EVENT_TABLE()

LoginFrame::~LoginFrame() {
}

void LoginFrame::LoadOptions() {
  Options::Init();
  wxTextCtrl *text;
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  text->ChangeValue(Options::name());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  text->ChangeValue("*****");
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  text->ChangeValue(Options::server());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  text->ChangeValue(Options::port());
}

void LoginFrame::SaveOptions() {
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Saving options"));

  wxTextCtrl *text;
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  Options::set_name(text->GetValue().ToStdString());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  if (text->GetValue().ToStdString() != "*****") {
    Options::set_password_hash(sha256(text->GetValue().
                                      ToStdString()));
    LOG4CPLUS_DEBUG(client_logger,
		    WStringFromUTF8String("Password changed to: " 
					  + text->GetValue().ToStdString()));
  }
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  Options::set_server(text->GetValue().ToStdString());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  Options::set_port(text->GetValue().ToStdString());
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Options saved"));
  
}

void LoginFrame::Init() {
  Centre();
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&LoginFrame::OnOKButtonClick);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&LoginFrame::OnExitButtonClick);
  LoadOptions();
}

void LoginFrame::OnOKButtonClick(wxCommandEvent& WXUNUSED(event)) {
  SaveOptions();
  ClientP client = CreateClient(Options::server(), Options::port());
  Options::set_client(client);
  LoginRequestP login(new LoginRequest);
  login->set_name(Options::name());
  login->set_password_hash(Options::password_hash());
  LOG4CPLUS_INFO(client_logger,
		 WStringFromUTF8String("Trying to log in"));

  MessageP ret = client->EvalRequest(*login);

  if (LoginResponseP resp = boost::dynamic_pointer_cast<LoginResponse>(ret)) {
    Options::set_admin(resp->admin());

    LOG4CPLUS_INFO(client_logger,
		   WStringFromUTF8String("Loged in"));
      
    grid_frame = new GridFrame();
    wxXmlResource::Get()->LoadFrame(grid_frame, NULL, _T("GridFrame"));
    grid_frame->Init();
    grid_frame->SetTitle(_T("TMS"));
    grid_frame->Show(true);
    LOG4CPLUS_INFO(client_logger,
		   WStringFromUTF8String("Determening user rights"));

    if (resp->admin()) {
      LOG4CPLUS_INFO(client_logger,
		     WStringFromUTF8String("User has admin rights"));
    }

    Close();
  } else {
      LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Logging in failed"));
    wxMessageDialog *msg = new wxMessageDialog(this, wxString::FromUTF8("Неверный логин или пароль"), wxString::FromUTF8("Ошибка"));
    msg->ShowModal();
  }
}

void LoginFrame::OnExitButtonClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
