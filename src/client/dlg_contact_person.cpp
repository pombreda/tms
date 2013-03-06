
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

BEGIN_EVENT_TABLE(DlgContactPerson, wxDialog)
END_EVENT_TABLE()

DlgContactPerson::~DlgContactPerson() {
}

DlgContactPerson::DlgContactPerson(wxWindow *parent) :
    wxDialog(), contraption_(), contraptions_() {
  if (!wxXmlResource::Get()->LoadDialog(this, parent,
                                        _T("dlgContactPerson"))) {
    LOG4CPLUS_ERROR(client_logger, 
                    WStringFromUTF8String("Can't load dlgContactPerson"));
  }
  
  Init();
}


void DlgContactPerson::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgContactPerson"));

  XRCCTRL(*this, "txtName", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "name"),
                      ContraptionSetter<std::string>(contraption_, "name")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Name binded"));
  
  XRCCTRL(*this, "txtCityCode", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "code"),
                      ContraptionSetter<std::string>(contraption_, "code")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Code binded"));

  XRCCTRL(*this, "txtEmail", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "email"),
                      ContraptionSetter<std::string>(contraption_, "email")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Email binded"));

  XRCCTRL(*this, "txtPhone", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "phone"),
                      ContraptionSetter<std::string>(contraption_, "phone")));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Phone binded"));
  
  XRCCTRL(*this, "txtNote", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "note"),
                      ContraptionSetter<std::string>(contraption_, "note"))); 
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Note binded"));
  
  XRCCTRL(*this, "txtCompany", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "company_name"),
                      DummySetter<std::string>()));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Company binded"));
  
  XRCCTRL(*this, "txtFax", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "fax"),
                      ContraptionSetter<std::string>(contraption_, "fax"))); 
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Fax binded"));
  
  XRCCTRL(*this, "txtPosition", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "role"),
                      ContraptionSetter<std::string>(contraption_, "role"))); 
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Role binded"));

  button_remove_ = XRCCTRL(*this, "btnDelete", wxButton);
  button_save_ = XRCCTRL(*this, "btnSave", wxButton);
  button_cancel_ = XRCCTRL(*this, "btnCancel", wxButton);
  Bind(wxEVT_CLOSE_WINDOW, &DlgContactPerson::OnTryClose, this);

  button_save_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                        (wxObjectEventFunction)&DlgContactPerson::OnSaveClick, 0, this);
  button_remove_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                          (wxObjectEventFunction)&DlgContactPerson::OnDeleteClick, 0, this);
  button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                          (wxObjectEventFunction)&DlgContactPerson::OnExitClick, 0, this);

  LOG4CPLUS_INFO(client_logger, 
  WStringFromUTF8String("DlgContactPerson initialized"));
}

void DlgContactPerson::SetUpValues(ContraptionP contraption,
                                   ContraptionArrayP contraptions, bool editable) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting contact person values"));
  contraption_ = contraption;
    
  contraptions_ = contraptions;
  button_remove_->Show(true);
  button_save_->Show(true);
  if (contraption->IsNew()) {
    button_remove_->Show(false);
    contraption->Save();
    button_cancel_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
    button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
			    (wxObjectEventFunction)&DlgContactPerson::OnDeleteClick, 0, this);
  } else {
    button_cancel_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
    button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
			    (wxObjectEventFunction)&DlgContactPerson::OnExitClick, 0, this);
  }
  if (!editable) {
    button_remove_->Show(false);
    button_save_->Show(false);
  }

  TransferDataToWindow();
    
  Fit();
  Layout();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Contact person values set"));
}

void DlgContactPerson::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving contact person values"));  
  if (!Validate() || !TransferDataFromWindow()) {
    return;
  }
  contraption_->Save();
  contraptions_->Refresh();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Contact person values saved"));  
  EndModal(wxOK);
}

void DlgContactPerson::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Contact person removed"));  
  EndModal(wxOK);
}

void DlgContactPerson::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  EndModal(wxOK);
}

void DlgContactPerson::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  wxCloseEvent new_event(wxEVT_COMMAND_BUTTON_CLICKED);
  button_cancel_->ProcessWindowEvent(new_event);
}

}
}
