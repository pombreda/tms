#ifndef _TMS_CLIENT__GRID_FRAME_HPP_
#define _TMS_CLIENT__GRID_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/choice.h>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
// std
#include <string>
#include <vector>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/server_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/contraption_array.hpp>
#include <widget/contraption_grid.hpp>
#include <model/user.hpp>
// frames
#include "contact_persons_frame.hpp"
#include "companies_frame.hpp"
#include "users_frame.hpp"
#include "incomings_frame.hpp"
#include "frames_collection.hpp"
// project
#include <project/model/contact_person.hpp>
#include <project/model/company.hpp>
#include <project/model/incoming.hpp>

namespace tms {
namespace client {

class GridFrame : public wxFrame {
 public:
  GridFrame() :
      wxFrame(),
      grid_books_(), choice_book_(), selected_book_id_(0),
      grid_catalogs_(), choice_catalog_(), selected_catalog_id_(0),
      table_contact_persons_() {}
  virtual ~GridFrame();
  void Init();

 private:
  GridFrame(const GridFrame&);
  GridFrame& operator=(const GridFrame&);

  tms::common::widget::ContraptionGrid *grid_books_;
  wxChoice *choice_book_;
  wxButton *button_add_in_book_;
  int selected_book_id_;
  tms::common::widget::ContraptionGridTableBase *table_incomings_;

  tms::common::widget::ContraptionGrid *grid_admin_;
  wxChoice *choice_admin_;
  wxButton *button_add_in_admin_;
  int selected_admin_id_;
  tms::common::widget::ContraptionGridTableBase *table_users_;
 
  tms::common::widget::ContraptionGrid *grid_catalogs_;
  wxChoice *choice_catalog_;
  wxButton *button_add_in_catalog_;
  int selected_catalog_id_;
  tms::common::widget::ContraptionGridTableBase *table_contact_persons_;
  tms::common::widget::ContraptionGridTableBase *table_companies_;


  void InitContactPersonsTable();
  void ActivateContactPersonsTable();
  void InitCompaniesTable();
  void ActivateCompaniesTable();
  void InitUsersTable();
  void ActivateUsersTable();
  void InitIncomingsTable();
  void ActivateIncomingsTable();
  void PrepareModels();

  void OnContactPersonsCellClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);
  void OnContactPersonsCellDClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);
  void OnCompaniesCellClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);
  void OnCompaniesCellDClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);

  void OnIncomingsCellClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);
  void OnIncomingsCellDClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);

  void OnUsersCellClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);
  void OnUsersCellDClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);

  void OnAddInIncomingClick(wxCommandEvent& WXUNUSED(event));

  void OnAddInContactPersonClick(wxCommandEvent& WXUNUSED(event));
  void OnAddInCompanyClick(wxCommandEvent& WXUNUSED(event));


  void OnAddInUserClick(wxCommandEvent& WXUNUSED(event));
  void OnPatchClick(wxCommandEvent& WXUNUSED(event));
  void OnCatalogSelect(wxCommandEvent& WXUNUSED(event));
  void OnBookSelect(wxCommandEvent& WXUNUSED(event));
  void OnAdminSelect(wxCommandEvent& WXUNUSED(event));
  void OnExitClick(wxCommandEvent& WXUNUSED(event));

  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__GRID_FRAME_HPP_
