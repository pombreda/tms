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

class TestFrame : public wxFrame {
 public:
  TestFrame() :
      wxFrame(), tc_name_(), tc_surname_(),
      tc_age_(), contraption_(), contraptions_() {}
	virtual ~TestFrame();
	void Init();
	void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);

 private:
  TestFrame(const TestFrame&);
  TestFrame& operator=(const TestFrame&);

  wxTextCtrl *tc_name_;
  wxTextCtrl *tc_surname_;
  wxTextCtrl *tc_age_;
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
