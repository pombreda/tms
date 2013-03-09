#ifndef _TMS_CLIENT__DLG_INCOMING_HPP_
#define _TMS_CLIENT__DLG_INCOMING_HPP_

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

class DlgIncoming : public tms::common::widget::ContraptionDialog {
 public:
  DlgIncoming(wxWindow *parent);
  virtual ~DlgIncoming();
 private:
  void Init();
  DlgIncoming(const DlgIncoming&);
  DlgIncoming& operator=(const DlgIncoming&);
};

}
}

#endif // _TMS_CLIENT__DLG_INCOMING_HPP_
