#ifndef _TMS_CLIENT__DLG_USER_HPP_
#define _TMS_CLIENT__DLG_USER_HPP_

// wx
#include <wx/window.h>
// common
#include <widget/contraption_dialog.hpp>

namespace tms {
namespace client {

class DlgUser : public tms::common::widget::ContraptionDialog {
 public:
  DlgUser(wxWindow *parent);
  virtual ~DlgUser();
 private:
  void Init();
  DlgUser(const DlgUser&);
  DlgUser& operator=(const DlgUser&);
};

}
}

#endif // _TMS_CLIENT__DLG_USER_HPP_
