#include "dlg_user.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// boost
#include <boost/bind.hpp>
// common
#include <protocol/crypto.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/bool_validator.hpp>
#include <widget/validators/password_validator.hpp>
#include <widget/validators/hide_if_validator.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget::validators;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;

DlgUser::~DlgUser() {
}

DlgUser::DlgUser(wxWindow *parent) :
    ContraptionDialog() {
  wxXmlResource::Get()->LoadDialog(this, parent,
                                   _T("dlgUser"));
  Init();
}

void DlgUser::Init() {

  XRCCTRL(*this, "txtName", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "name"),
                      ContraptionSetter<std::string>(contraption_, "name")));
  XRCCTRL(*this, "txtPassword", wxTextCtrl)->SetValidator(
      PasswordValidator(ContraptionSetter<std::string>(contraption_, "password_hash")));
  XRCCTRL(*this, "chkAdmin", wxCheckBox)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "admin"),
                    ContraptionSetter<bool>(contraption_, "admin")));
  XRCCTRL(*this, "chkSecretary", wxCheckBox)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "secretary"),
                    ContraptionSetter<bool>(contraption_, "secretary")));
  XRCCTRL(*this, "chkManager", wxCheckBox)->SetValidator(
      BoolValidator(ContraptionGetter<bool>(contraption_, "manager"),
                    ContraptionSetter<bool>(contraption_, "manager")));
  SetSaveButton(XRCCTRL(*this, "btnSave", wxButton));
  SetDeleteButton(XRCCTRL(*this, "btnDelete", wxButton));
  SetCancelButton(XRCCTRL(*this, "btnCancel", wxButton));
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgUser Initialized"));
}


}
}
