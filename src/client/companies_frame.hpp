#ifndef _TMS_CLIENT__COMPANIES_FRAME_HPP_
#define _TMS_CLIENT__COMPANIES_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/choice.h>
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
#include <widget/contraption_grid.hpp>
// frames
#include "dlg_contact_person.hpp"
namespace tms {
namespace client {

class CompaniesFrame : public wxFrame {
 public:
  CompaniesFrame() :
    wxFrame(), tc_id_(), tc_full_name_(), tc_short_name_(),
    tc_gruppa_(), tc_okonh_(), tc_ogrn_(), 
    tc_bank_(), tc_bank_city_(), tc_lits_schet_(), tc_cor_schet_(),
    tc_inn_(), tc_bik_(), 
    choice_contact_person_(),
    button_add_in_contact_person_(), grid_contact_persons_(),
    table_contact_persons_(),
    button_remove_(), button_cancel_(),
    timer_(new wxTimer(this)),
    contraption_(), contraptions_() {}
  virtual ~CompaniesFrame();
  void Init();
  void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);

 private:
  CompaniesFrame(const CompaniesFrame&);
  CompaniesFrame& operator=(const CompaniesFrame&);
  // general
  wxTextCtrl *tc_id_;
  wxTextCtrl *tc_full_name_;
  wxTextCtrl *tc_short_name_;
  wxTextCtrl *tc_gruppa_;
  wxTextCtrl *tc_okonh_;
  wxTextCtrl *tc_ogrn_;
  // bank
  wxTextCtrl *tc_bank_;
  wxTextCtrl *tc_bank_city_;
  wxTextCtrl *tc_lits_schet_;
  wxTextCtrl *tc_cor_schet_;
  wxTextCtrl *tc_inn_;
  wxTextCtrl *tc_bik_;
  // personal
  wxChoice *choice_contact_person_;
  wxButton *button_add_in_contact_person_;
  tms::common::widget::ContraptionGrid *grid_contact_persons_;
  tms::common::widget::ContraptionGridTableBase *table_contact_persons_;
  // contacts
  wxTextCtrl *tc_yur_addr_;
  wxTextCtrl *tc_fact_addr_;
  wxTextCtrl *tc_mail_;
  wxTextCtrl *tc_phone_;
  // other
  wxTextCtrl *tc_comment_;
  
  wxButton *button_remove_;
  wxButton *button_cancel_;
  wxTimer *timer_;
  DlgContactPerson *dlg_contact_person_;
  int cur_contact_person_id_;
  tms::common::contraption::ContraptionP contraption_;
  tms::common::contraption::ContraptionArrayP contraptions_;
  tms::common::contraption::ContraptionArrayP contact_persons_;
  std::vector<int> contact_persons_id_;

  void OnTimerContactPerson(wxTimerEvent& WXUNUSED(event));
  void InitChoiceContactPersson(int contact_person_id);
  void OnSaveClick(wxCommandEvent& WXUNUSED(event));
  void OnDeleteClick(wxCommandEvent& WXUNUSED(event));
  void OnExitClick(wxCommandEvent& WXUNUSED(event));
  void OnTryClose(wxCloseEvent& event);
  void OnAddInContactPersonClick(wxCommandEvent& WXUNUSED(event));
  void OnContactPersonsCellClick(tms::common::contraption::ContraptionP contraption,
				 tms::common::contraption::FieldID field_id);
  void OnContactPersonsCellDClick(tms::common::contraption::ContraptionP contraption,
				  tms::common::contraption::FieldID field_id);


  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__COMPANIES_FRAME_HPP_
