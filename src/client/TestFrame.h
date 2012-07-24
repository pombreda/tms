#ifndef TESTFRAME_H
#define TESTFRAME_H

#include <wx/frame.h>
#include <wx/textctrl.h>

#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>

#include <string>
#include <boost/lexical_cast.hpp>

using namespace tms::common::contraption;

class TestFrame: public wxFrame {
 public:
  TestFrame() :
    wxFrame(), tc_name_(), tc_surname_(),
    tc_age_(), contraption_() {}
	virtual ~TestFrame();
	void Init();
	void SetUpValues(ContraptionP contraption, ContraptionArrayP contraptions);

 private:
  TestFrame(const TestFrame&);
  TestFrame& operator=(const TestFrame&);

  wxTextCtrl *tc_name_;
  wxTextCtrl *tc_surname_;
  wxTextCtrl *tc_age_;
  ContraptionP contraption_;
  ContraptionArrayP contraptions_;

	void OnSaveClick(wxCommandEvent& WXUNUSED(event));
	void OnDeleteClick(wxCommandEvent& WXUNUSED(event));
	void OnExitClick(wxCommandEvent& WXUNUSED(event));

	DECLARE_EVENT_TABLE()
};

#endif
