#ifndef _TMS_CLIENT__LOGIN_FRAME_HPP_
#define _TMS_CLIENT__LOGIN_FRAME_HPP_

// wxWidgets
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
// common
#include "grid_frame.hpp"
#include "options.hpp"

namespace tms {
namespace client{

class LoginFrame : public wxFrame {
 public:
  LoginFrame() :
      wxFrame(), grid_frame() {}
	virtual ~LoginFrame();
  void Init();

 private:
  LoginFrame(const LoginFrame&);
  LoginFrame& operator=(const LoginFrame&);

  GridFrame *grid_frame;
  tms::common::widget::ContraptionGridTableBase *base_;

  void OnOKButtonClick(wxCommandEvent& event);
  void OnExitButtonClick(wxCommandEvent& event);
  void LoadOptions();
  void SaveOptions();

  DECLARE_EVENT_TABLE()
};

}
}
#endif // _TMS_CLIENT__LOGIN_FRAME_HPP_
