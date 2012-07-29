#include "login_frame.hpp"
// common
#include <protocol/client.hpp>
#include <protocol/crypto.hpp>
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <contraption/model_backend/server_model_backend.hpp>
#include <contraption/model.hpp>
#include <widget/contraption_grid_table_base.hpp>
// client
#include <client/client.hpp>
// project
#include <project/model/contact_person.hpp>

#include <wx/grid.h>

namespace tms {
namespace client {

using namespace tms::common::protocol;
using namespace tms::common::contraption;
using namespace tms::common::protocol::message;
using namespace tms::project::model;
using namespace tms::common::widget;

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
  wxTextCtrl *text;
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL1");
  Options::set_name(text->GetValue().ToStdString());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL2");
  if (text->GetValue().ToStdString() != "*****") {
    Options::set_password_hash(sha256(text->GetValue().
                                      ToStdString()));
  }
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL3");
  Options::set_server(text->GetValue().ToStdString());
  text = (wxTextCtrl*)FindWindowByName("ID_TEXTCTRL4");
  Options::set_port(text->GetValue().ToStdString());
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
  MessageP ret = client->EvalRequest(*login);
  if (boost::dynamic_pointer_cast<LoginResponse>(ret)) {
    grid_frame = new GridFrame();
    wxXmlResource::Get()->LoadFrame(grid_frame, NULL, _T("GridFrame"));
    grid_frame->Init();
    grid_frame->SetTitle(_T("TMS"));
    grid_frame->Show(true);
  }
  Close();
}

void LoginFrame::OnExitButtonClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
