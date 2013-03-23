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
#include <project/model/subject.hpp>
#include <project/model/addressee.hpp>

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
  FrmGrid(const FrmGrid&);
  FrmGrid& operator=(const FrmGrid&);

  tms::common::widget::ContraptionGrid *grid_books_;
  tms::common::widget::ContraptionGridTableBase *table_incomings_;

  tms::common::widget::ContraptionGrid *grid_admin_;
  tms::common::widget::ContraptionGridTableBase *table_users_;
  tms::common::widget::ContraptionGridTableBase *table_subjects_;
  tms::common::widget::ContraptionGridTableBase *table_addressees_;
 
  tms::common::widget::ContraptionGrid *grid_catalogs_;
  tms::common::widget::ContraptionGridTableBase *table_contact_persons_;
  tms::common::widget::ContraptionGridTableBase *table_companies_;


  void InitContactPersonsTable();
  void InitCompaniesTable();
  void InitUsersTable();
  void InitSubjectsTable();
  void InitAddresseesTable();
  void InitIncomingsTable();
  void PrepareModels();

  void OnImportIncomingClick();
  void OnPatchClick(wxCommandEvent& WXUNUSED(event));
  void OnClose(wxCloseEvent& event);
  void OnExitClick(wxCommandEvent& WXUNUSED(event));
};

}
}

#endif // _TMS_CLIENT__FRM_GRID_HPP_
