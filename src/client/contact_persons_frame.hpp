#ifndef _TMS_CLIENT__TEST_FRAME_HPP_
#define _TMS_CLIENT__TEST_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
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

class ContactPersonsFrame : public wxFrame {
 public:
  ContactPersonsFrame() :
      wxFrame(), tc_name_(), tc_code_(), tc_email_(),
      tc_phone_(), tc_fax_(), tc_notes_(),
      contraption_(), contraptions_() {}
	virtual ~ContactPersonsFrame();
	void Init();
	void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);

 private:
  ContactPersonsFrame(const ContactPersonsFrame&);
  ContactPersonsFrame& operator=(const ContactPersonsFrame&);

  wxTextCtrl *tc_name_;
  wxTextCtrl *tc_code_;
  wxTextCtrl *tc_email_;
  wxTextCtrl *tc_phone_;
  wxTextCtrl *tc_fax_;
  wxTextCtrl *tc_notes_;
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

#endif // _TMS_CLIENT__TEST_FRAME_HPP_
