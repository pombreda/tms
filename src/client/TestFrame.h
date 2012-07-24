#ifndef TESTFRAME_H
#define TESTFRAME_H

#include <wx/frame.h>
#include <wx/textctrl.h>

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

class TestFrame: public wxFrame {
 public:
  TestFrame() :
    wxFrame(), tc_name_(), tc_surname_(), tc_age_() {}
	virtual ~TestFrame();
	void Init();
	void SetUpValues(const std::string &name, const std::string &surname,
                   int age);

 private:
  typedef boost::shared_ptr<wxTextCtrl> wxTextCtrlP;

  TestFrame(const TestFrame&);
  TestFrame& operator=(const TestFrame&);

  wxTextCtrl *tc_name_;
  wxTextCtrl *tc_surname_;
  wxTextCtrl *tc_age_;

	void OnSaveClick(wxCommandEvent& WXUNUSED(event));
	void OnDeleteClick(wxCommandEvent& WXUNUSED(event));
	void OnExitClick(wxCommandEvent& WXUNUSED(event));

	DECLARE_EVENT_TABLE()
};

#endif
