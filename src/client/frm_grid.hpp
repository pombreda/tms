#ifndef _TMS_CLIENT__FRM_GRID_HPP_
#define _TMS_CLIENT__FRM_GRID_HPP_

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
// project
#include <project/model/contact_person.hpp>
#include <project/model/company.hpp>
#include <project/model/incoming.hpp>

namespace tms {
namespace client {

class FrmGrid : public wxFrame {
 public:
  FrmGrid();
  virtual ~FrmGrid();
  void Init();

 private:
  void InitDialogs();
  void InitBooks();
  void InitCatalogs();
  void InitAdmin();
  void DeactivateBooks();
  void DeactivateAdmin();
  void DeactivateCatalogs();
  FrmGrid(const FrmGrid&);
  FrmGrid& operator=(const FrmGrid&);

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
  void DeactivateContactPersonsTable();
  void InitCompaniesTable();
  void ActivateCompaniesTable();
  void DeactivateCompaniesTable();
  void InitUsersTable();
  void ActivateUsersTable();
  void DeactivateUsersTable();
  void InitIncomingsTable();
  void ActivateIncomingsTable();
  void DeactivateIncomingsTable();
  void PrepareModels();


  void OnBooksLabelRightClick(wxGridEvent &e);
  void OnContactPersonsCellDClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);

  void OnCompaniesCellDClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);

  void OnIncomingsCellDClick(tms::common::contraption::ContraptionP contraption,
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
  void OnClose(wxCloseEvent& event);

  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__FRM_GRID_HPP_
