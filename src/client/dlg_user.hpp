#ifndef _TMS_CLIENT__DLG_USER_HPP_
#define _TMS_CLIENT__DLG_USER_HPP_

// wxWidgets
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/xrc/xmlres.h>
// boost
#include <boost/lexical_cast.hpp>
// std
#include <string>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>
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
