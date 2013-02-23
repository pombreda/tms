#ifndef _TMS_CLIENT__USERS_FRAME_HPP_
#define _TMS_CLIENT__USERS_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/xrc/xmlres.h>
// boost
#include <boost/lexical_cast.hpp>
// std
#include <string>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>

namespace tms {
namespace client {

class UsersFrame : public wxFrame {
 public:
  UsersFrame() :
      wxFrame(), tc_name_(), tc_password_(), cb_admin_(),
      cb_secretair_(), contraption_(), contraptions_() {}
  virtual ~UsersFrame();
  void Init();
  void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);

 private:
  UsersFrame(const UsersFrame&);
  UsersFrame& operator=(const UsersFrame&);

  wxTextCtrl *tc_name_;
  wxTextCtrl *tc_password_;
  wxCheckBox *cb_admin_;
  wxCheckBox *cb_secretair_;
  wxCheckBox *cb_manager_;
  wxButton *button_remove_;
  tms::common::contraption::ContraptionP contraption_;
  tms::common::contraption::ContraptionArrayP contraptions_;

  void OnSaveClick(wxCommandEvent& WXUNUSED(event));
  void OnDeleteClick(wxCommandEvent& WXUNUSED(event));
  void OnExitClick(wxCommandEvent& WXUNUSED(event));
  void OnTryClose(wxCloseEvent& event);

  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__USERS_FRAME_HPP_
