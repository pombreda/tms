#ifndef _TMS_CLIENT__INCOMINGS_FRAME_HPP_
#define _TMS_CLIENT__INCOMINGS_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/xrc/xmlres.h>
// boost
#include <boost/lexical_cast.hpp>
// std
#include <string>
#include <vector>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>
// frames
#include "companies_frame.hpp"
#include "frames_collection.hpp"

namespace tms {
namespace client {

class IncomingsFrame : public wxFrame {
 public:
  IncomingsFrame() :
    wxFrame(), tc_id_(), tc_time_in_(),
    tc_topic_(), tc_time_out_(), choice_company_(),
    choice_contact_person_(), choice_type_in_(),
    button_send_(), button_remove_(),
    button_save_(), button_cancel_(),
    button_add_company_(), button_add_contact_person_(),
    add_company_timer_(new wxTimer(this)),
    contraption_(), contraptions_(){}
  virtual ~IncomingsFrame();
  void Init();
  void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);

 private:
  IncomingsFrame(const IncomingsFrame&);
  IncomingsFrame& operator=(const IncomingsFrame&);

  wxTextCtrl *tc_id_;
  wxTextCtrl *tc_time_in_;
  wxTextCtrl *tc_topic_;
  wxTextCtrl *tc_time_out_;
  wxChoice *choice_company_;
  wxChoice *choice_contact_person_;
  wxChoice *choice_manager_;
  wxChoice *choice_type_in_;
  wxButton *button_send_;
  wxButton *button_remove_;
  wxButton *button_save_;
  wxButton *button_cancel_;
  wxButton *button_add_company_;
  wxButton *button_add_contact_person_;
  wxTimer * add_company_timer_;

  tms::common::contraption::ContraptionP contraption_;
  tms::common::contraption::ContraptionArrayP contraptions_;
  tms::common::contraption::ContraptionP company_;
  tms::common::contraption::ContraptionArrayP companies_;
  tms::common::contraption::ContraptionArrayP contact_persons_;
  tms::common::contraption::ContraptionP contact_person_;
  tms::common::contraption::ContraptionArrayP users_;
  std::vector<int> contact_persons_id_;

  void OnSendClick(wxCommandEvent& WXUNUSED(event));
  void OnAddCompanyClick(wxCommandEvent& WXUNUSED(event));
  void OnAddContactPersonClick(wxCommandEvent& WXUNUSED(event));
  void OnCompanyChoice(wxCommandEvent& WXUNUSED(event));
  void OnAddCompanyTimer(wxTimerEvent& WXUNUSED(event));
  void OnAddContactPersonTimer(wxTimerEvent& WXUNUSED(event));
  void OnSaveClick(wxCommandEvent& WXUNUSED(event));
  void OnDeleteClick(wxCommandEvent& WXUNUSED(event));
  void OnExitClick(wxCommandEvent& WXUNUSED(event));
  void OnTryClose(wxCloseEvent& event);

  void InitChoiceContactPerson(int contact_person_id);
  void InitChoiceCompany(int company_id);
  void InitChoiceManager(int manager_id);
  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__INCOMINGS_FRAME_HPP_
