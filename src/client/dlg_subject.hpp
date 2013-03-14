#ifndef _TMS_CLIENT__DLG_SUBJECTS_HPP_
#define _TMS_CLIENT__DLG_SUBJECTS_HPP_

// wx
#include <wx/window.h>
// common
#include <widget/contraption_dialog.hpp>

namespace tms {
namespace client {

class DlgSubject : public common::widget::ContraptionDialog {
 public:
  DlgSubject(wxWindow *parent);
  virtual ~DlgSubject();
 private:
  void Init();
  DlgSubject(const DlgSubject&);
  DlgSubject& operator=(const DlgSubject&);
};

}
}
#endif // _TMS_CLIENT__DLG_SUBJECTS_HPP_
