#include "dlg_login.hpp"
// log4cplus
#include <client/logger.hpp>
// log4cplus
#include <log4cplus/configurator.h>
// std
#include <fstream>
// common
#include <protocol/client.hpp>
#include <protocol/message/login_request.hpp>
#include <protocol/message/patch_client_request.hpp>
#include <protocol/message/patch_client_response.hpp>
#include <patcher/patcher.hpp>
#include <contraption/model_backend/server_model_backend.hpp>
#include <contraption/model.hpp>
#include <string/string.hpp>
#include <widget/contraption_grid_table_base.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/password_validator.hpp>
#include <config.hpp>
// client
#include <client/client.hpp>
#include <client/options.hpp>
// project
#include <project/model/contact_person.hpp>
// wx
#include <wx/msgdlg.h>
#include <wx/grid.h>
#include <wx/app.h>
// boost
#include <boost/filesystem.hpp>

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace tms::common::protocol;
using namespace tms::common::contraption;
using namespace tms::common::protocol::message;
using namespace tms::project::model;
using namespace tms::common::widget;
using namespace tms::common::widget::validators;
using namespace tms::common::string;
using namespace tms::common::patcher;
using namespace boost::filesystem;

BEGIN_EVENT_TABLE(DlgLogin,wxDialog)
END_EVENT_TABLE()

DlgLogin::DlgLogin() :
  wxDialog(), grid_frame() {
  wxXmlResource::Get()->LoadDialog(this, 0, _T("dlgLogin"));
  Init();
}

DlgLogin::~DlgLogin() {
}

void DlgLogin::Init() {
  Centre();
  XRCCTRL(*this, "txtLogin", wxTextCtrl)->SetValidator(
      StringValidator(Options::name, Options::set_name));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Login binded"));
    
  XRCCTRL(*this, "txtIP", wxTextCtrl)->SetValidator(
      StringValidator(Options::server, Options::set_server));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("IP binded"));
    
  XRCCTRL(*this, "txtPort", wxTextCtrl)->SetValidator(
      StringValidator(Options::port, Options::set_port));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Port binded"));
  
  XRCCTRL(*this, "txtPassword", wxTextCtrl)->SetValidator(
      PasswordValidator(Options::set_password_hash));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Password binded"));

  XRCCTRL(*this, "btnLogin", wxButton)->
      Connect(wxEVT_COMMAND_BUTTON_CLICKED,
              (wxObjectEventFunction)&DlgLogin::OnOKButtonClick,
              0,
              this);
  Options::Init();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgLogin initialized"));
  
  TransferDataToWindow();
  Bind(wxEVT_CLOSE_WINDOW, &DlgLogin::OnClose, this);  
}

void DlgLogin::Patch() {
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Testing if a patch needed"));
  PatchClientRequestP patch_request(new PatchClientRequest());
  patch_request->set_version(kVersion);
  patch_request->set_system(kSystem);
  ClientP client = Options::client();
  MessageP ret = client->EvalRequest(*patch_request);
  if (PatchClientResponseP resp = boost::dynamic_pointer_cast<PatchClientResponse>(ret)) {
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("Patch needed"));
    if (resp->patch_archive().size() == 0) {
      wxMessageDialog(this,
                      WStringFromUTF8String("Данная версия клиента больше не поддержмвается.")).ShowModal();
    } else {
      wxMessageDialog(this,
                      WStringFromUTF8String("Запускается обновление системы.")).ShowModal();
		
      ofstream fout("latest.patch", ios::out | ios::binary);
      fout.write(resp->patch_archive().c_str(), resp->patch_archive().size());
      fout.close();
      tms::common::patcher::Patch(path("latest.patch"), current_path());  
      wxMessageDialog(this,
                      WStringFromUTF8String("Обновленме завершено. Перезапустите приложение.")).ShowModal();
    }
    EndModal(wxOK);
    return;
  } 
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Patch not needed"));      
}

void DlgLogin::SetPermissions(LoginResponseP resp) {
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Determening user permissions"));

  if (resp->admin()) {
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("User has admin rights"));
  }
    
  Options::set_admin(resp->admin());

  if (resp->secretary()) {
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("User has secretary rights"));
  }
    
  Options::set_secretary(resp->secretary());

  if (resp->manager()) {
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("User has manager rights"));
  }
    
  Options::set_manager(resp->manager());
}

void DlgLogin::TryLogin() {
  ClientP client = Options::client();
  LoginRequestP login(new LoginRequest);
  login->set_name(Options::name());
  login->set_password_hash(Options::password_hash());
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Trying to log in"));
    
  MessageP ret = client->EvalRequest(*login);
    
  if (LoginResponseP resp = boost::dynamic_pointer_cast<LoginResponse>(ret)) {
    Patch();
    SetPermissions(resp);
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("Loged in"));
    EndModal(wxOK);
  } else {
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("Logging in failed"));

    wxMessageDialog *msg = new wxMessageDialog(this,
                                               wxString::FromUTF8("Неверный логин или пароль"),
                                               wxString::FromUTF8("Ошибка"));
    msg->ShowModal();
  }
}

void DlgLogin::OnOKButtonClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger,
                 WStringFromUTF8String("Saving options"));

  if (Validate() && TransferDataFromWindow()) {
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("Options saved"));
    
    ClientP client = CreateClient(Options::server(), Options::port());
    Options::set_client(client);

    TryLogin();
  }

}

void DlgLogin::OnExitButtonClick(wxCommandEvent& WXUNUSED(event)) {
  EndModal(wxCANCEL);
}

void DlgLogin::OnClose(wxCloseEvent& event) {
  EndModal(wxCANCEL);
}

}
}
