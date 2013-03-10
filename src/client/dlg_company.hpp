#ifndef _TMS_CLIENT__DLG_COMPANY_HPP_
#define _TMS_CLIENT__DLG_COMPANY_HPP_

// wx
#include <wx/window.h>
// common
#include <widget/contraption_dialog.hpp>

namespace tms {
namespace client {

class DlgCompany : public tms::common::widget::ContraptionDialog {
 public:
  DlgCompany(wxWindow *parent);
  virtual ~DlgCompany();
 private:
  void Delete();
  std::string GetID();
  void Init();
  common::contraption::ContraptionArrayP ContactPersons();
  common::contraption::ContraptionP ContactPersonFactory();
  DlgCompany(const DlgCompany&);
  DlgCompany& operator=(const DlgCompany&);
};

}
}
#endif // _TMS_CLIENT__DLG_COMPANY_HPP_
