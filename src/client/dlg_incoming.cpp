#include "dlg_incoming.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>
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
#include <widget/contraption_choice.hpp>
// project
// project
#include <project/model/company.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::project::model;
using namespace tms::common::widget;
using namespace tms::common::widget::validators;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;

DlgIncoming::~DlgIncoming() {
}

DlgIncoming::DlgIncoming(wxWindow *parent) :
    ContraptionDialog() {
  wxXmlResource::Get()->LoadDialog(this, parent,
                                   _T("dlgIncoming"));
  Init();
}

void DlgIncoming::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgIncoming"));
  ContraptionChoice *choice_company = 
      new ContraptionChoice(this, 
                            boost::bind(
                                static_cast
                                <std::string (Contraption::*)(const std::string&)>(
                                    &Contraption::Get<std::string>),
                                _1, std::string("short_name")));
  
  wxXmlResource::Get()->AttachUnknownControl("cchCompany", choice_company);
  
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgIncoming Initialized"));
}


}
}
