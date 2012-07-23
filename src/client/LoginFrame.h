#ifndef LOGINFRAME_H
#define LOGINFRAME_H

//(*Headers(LoginFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class LoginFrame: public wxFrame
{
	public:

		virtual ~LoginFrame();
  	void Init();

	protected:

	private:

		void OnOKButtonClick(wxCommandEvent& event);
		void OnExitButtonClick(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
