
#include "dlg_contact_person.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/dummy.hpp>
// boost
#include <boost/lexical_cast.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"
// wx
#include <wx/xrc/xmlres.h>
#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow


namespace tms {
namespace client {

using namespace tms::common::widget::validators;
using namespace tms::common::contraption;
using namespace tms::common::string;


DlgContactPerson::~DlgContactPerson() {
}

DlgContactPerson::DlgContactPerson(wxWindow *parent) :
    ContraptionDialog() {
  if (!wxXmlResource::Get()->LoadDialog(this, parent,
                                        _T("dlgContactPerson"))) {
    LOG4CPLUS_ERROR(logger_, 
                    WStringFromUTF8String("Can't load dialog"));    
  }
  
  Init();
}


void DlgContactPerson::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgContactPerson"));

  XRCCTRL(*this, "txtName", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "name"),
                      ContraptionSetter<std::string>(contraption_, "name")));
  XRCCTRL(*this, "txtCityCode", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "city_code"),
                      ContraptionSetter<std::string>(contraption_, "city_code")));
  XRCCTRL(*this, "txtEmail", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "email"),
                      ContraptionSetter<std::string>(contraption_, "email")));
  XRCCTRL(*this, "txtPhone", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "phone"),
                      ContraptionSetter<std::string>(contraption_, "phone")));
  XRCCTRL(*this, "txtNote", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "note"),
                      ContraptionSetter<std::string>(contraption_, "note"))); 
  XRCCTRL(*this, "txtCompany", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "company_name"),
                      DummySetter<std::string>()));
  XRCCTRL(*this, "txtFax", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "fax"),
                      ContraptionSetter<std::string>(contraption_, "fax"))); 
  XRCCTRL(*this, "txtPosition", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "position"),
                      ContraptionSetter<std::string>(contraption_, "position"))); 

  SetSaveButton(XRCCTRL(*this, "btnSave", wxButton));
  SetDeleteButton(XRCCTRL(*this, "btnDelete", wxButton));
  SetCancelButton(XRCCTRL(*this, "btnCancel", wxButton));

  LOG4CPLUS_INFO(client_logger, 
  WStringFromUTF8String("DlgContactPerson initialized"));
}


}
}
