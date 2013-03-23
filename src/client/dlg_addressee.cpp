#include "dlg_addressee.hpp"
// wx
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/xrc/xmlres.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace common::string;
using namespace common::widget::validators;

DlgAddressee::~DlgAddressee() {
}

DlgAddressee::DlgAddressee(wxWindow *parent):
    ContraptionDialog() {
  wxASSERT(wxXmlResource::Get()->LoadDialog(this, parent,
                                            _T("dlgAddressee")));
  Init();
}

void DlgAddressee::Init() {
    LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgAddressee"));
    XRCCTRL(*this, "txtAddressee", wxTextCtrl)->SetValidator(
        StringValidator(ContraptionGetter<std::string> (contraption_, "addressee"),
                        ContraptionSetter<std::string> (contraption_, "addressee")));
    SetSaveButton(XRCCTRL(*this, "btnSave", wxButton));
    SetDeleteButton(XRCCTRL(*this, "btnDelete", wxButton));
    SetCancelButton(XRCCTRL(*this, "btnCancel", wxButton));
    LOG4CPLUS_INFO(client_logger,
                   WStringFromUTF8String("DlgAddressee Initialized"));
}

}
}
