#ifndef _TMS_CLIENT__DLG_LOGIN_HPP_
#define _TMS_CLIENT__DLG_LOGIN_HPP_

// wxWidgets
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
// common
#include <protocol/message/login_response.hpp>
#include "frm_grid.hpp"

namespace tms {
namespace client{

class DlgLogin : public wxDialog {
 public:
  DlgLogin();
  virtual ~DlgLogin();
  void Init();

 private:
  DlgLogin(const DlgLogin&);
  DlgLogin& operator=(const DlgLogin&);

  FrmGrid *grid_frame;
  tms::common::widget::ContraptionGridTableBase *base_;

  void TryLogin();
  void Patch();
  void SetPermissions(tms::common::protocol::message::LoginResponseP resp);
  void OnOKButtonClick(wxCommandEvent& event);
  void OnExitButtonClick(wxCommandEvent& event);

  DECLARE_EVENT_TABLE()
};

}
}
#endif // _TMS_CLIENT__DLG_LOGIN_HPP_
