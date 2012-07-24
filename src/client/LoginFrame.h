#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "GridFrame.h"

class LoginFrame: public wxFrame {
 public:
  LoginFrame() :
    wxFrame(), grid_frame() {}
	virtual ~LoginFrame();
  void Init();

 private:
  GridFrame *grid_frame;

  void OnOKButtonClick(wxCommandEvent& event);
  void OnExitButtonClick(wxCommandEvent& event);

  DECLARE_EVENT_TABLE()
};

#endif
