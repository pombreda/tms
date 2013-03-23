#ifndef _TMS_CLIENT__DLG_ADDRESSEES_HPP_
#define _TMS_CLIENT__DLG_ADDRESSEES_HPP_

// wx
#include <wx/window.h>
// common
#include <widget/contraption_dialog.hpp>

namespace tms {
namespace client {

class DlgAddressee : public common::widget::ContraptionDialog {
 public:
  DlgAddressee(wxWindow *parent);
  virtual ~DlgAddressee();
 private:
  void Init();
  DlgAddressee(const DlgAddressee&);
  DlgAddressee& operator=(const DlgAddressee&);
};

}
}
#endif // _TMS_CLIENT__DLG_ADDRESSEES_HPP_
