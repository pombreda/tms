#ifndef _TMS_CLIENT__COMPANIES_FRAME_HPP_
#define _TMS_CLIENT__COMPANIES_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/choice.h>
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

class CompaniesFrame : public wxFrame {
 public:
  CompaniesFrame() :
    wxFrame(), tc_id_(), tc_full_name_(), tc_short_name_(),
    tc_gruppa_(), tc_okonh_(), tc_ogrn_(), 
    tc_bank_(), tc_bank_city_(), tc_lits_schet_(), tc_cor_schet_(),
    tc_inn_(), tc_bik_(), 
    choice_gen_dir_(), choice_glavbuh_(), choice_contact_person_(),
    button_add_in_contact_persons_(), grid_contact_persons_(),
    table_contact_persons_(),
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
  wxChoice *choice_gen_dir_;
  wxChoice *choice_glavbuh_;
  wxChoice *choice_contact_person_;
  wxButton *button_add_in_contact_persons_;
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

#endif // _TMS_CLIENT__COMPANIES_FRAME_HPP_
