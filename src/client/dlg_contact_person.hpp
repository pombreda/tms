#ifndef _TMS_CLIENT__CONTACT_PERSON_FRAME_HPP_
#define _TMS_CLIENT__CONTACT_PERSON_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/xrc/xmlres.h>
// boost
#include <boost/lexical_cast.hpp>
// std
#include <string>
// common
#include <widget/contraption_dialog.hpp>

namespace tms {
namespace client {

class DlgContactPerson : public tms::common::widget::ContraptionDialog {
 public:
  DlgContactPerson(wxWindow *parent);
  virtual ~DlgContactPerson();
  
  void Init();
 private:
  DlgContactPerson& operator=(const DlgContactPerson&);
  DlgContactPerson(const DlgContactPerson&);
};

}
}

#endif // _TMS_CLIENT__CONTACT_PERSON_FRAME_HPP_
