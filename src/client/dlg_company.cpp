#include "dlg_company.hpp"
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
#include <boost/lexical_cast.hpp>
// common
#include <protocol/crypto.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/bool_validator.hpp>
#include <widget/validators/password_validator.hpp>
#include <widget/validators/hide_if_validator.hpp>
#include <widget/validators/has_many_validator.hpp>
#include <widget/validators/has_one_validator.hpp>
#include <widget/validators/dummy.hpp>
#include <widget/contraption_choice.hpp>
#include <project/model/contact_person.hpp>
#include <project/model/company.hpp>
#include <widget/contraption_grid.hpp>
// frames
#include "dlg_contact_person.hpp"
#include "frames_collection.hpp"
#include "frm_grid.hpp"

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace tms::common::widget::validators;
using namespace tms::common::widget;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;
using namespace boost;

DlgCompany::~DlgCompany() {
}

DlgCompany::DlgCompany(wxWindow *parent) :
    ContraptionDialog() {
  wxXmlResource::Get()->LoadDialog(this, parent,
                                   _T("dlgCompany"));
  Init();
}

std::string DlgCompany::GetID() {
  std::string id = "0";
  if (contraptions_->size() > 0) {
    int p = contraptions_->size() - 1;
      while (p >= 0 && contraptions_->at(p)->Get<std::string>("ID") == "") {
	--p;
      }
      if (p >= 0) {
	ContraptionP last = contraptions_->at(p);
	std::string last_id = last->Get<std::string>("ID");
        try {
          id = lexical_cast<std::string>(1 + lexical_cast<int>(last_id));
        } catch (...) {
        }
      }
  }
  return id; 
}

ContraptionArrayP DlgCompany::ContactPersons() {
  return contraption_->Get<ContraptionArrayP>("contact_persons");
}

ContraptionP DlgCompany::ContactPersonFactory() {
  ContraptionP contact_person = ContactPerson::GetModel()->New();
  contact_person->Set<int>("company_id", contraption_->Get<int>("id"));
  return contact_person;
}

void DlgCompany::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgCompanys"));
  XRCCTRL(*this, "txtID", wxTextCtrl)->SetValidator(
      StringValidator(DefaultGetter<std::string>(
          ContraptionGetter<std::string>(contraption_, "ID"), 
          boost::bind(&DlgCompany::GetID, this)),
                      ContraptionSetter<std::string>(contraption_, "ID")));
  XRCCTRL(*this, "txtShortName", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "short_name"),
                      ContraptionSetter<std::string>(contraption_, "short_name")));  
  XRCCTRL(*this, "txtFullName", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "full_name"),
                      ContraptionSetter<std::string>(contraption_, "full_name")));  
  XRCCTRL(*this, "txtGroup", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "group"),
                      ContraptionSetter<std::string>(contraption_, "group")));  
  XRCCTRL(*this, "txtTIN", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "tin"),
                      ContraptionSetter<std::string>(contraption_, "tin")));  
  XRCCTRL(*this, "txtPSRN", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "psrn"),
                      ContraptionSetter<std::string>(contraption_, "psrn")));  
  XRCCTRL(*this, "txtBank", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "bank"),
                      ContraptionSetter<std::string>(contraption_, "bank")));  
  XRCCTRL(*this, "txtBankCity", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "bank_city"),
                      ContraptionSetter<std::string>(contraption_, "bank_city")));  
  XRCCTRL(*this, "txtPersonalAccount", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, 
                                                     "personal_account"),
                      ContraptionSetter<std::string>(contraption_, 
                                                     "personal_account")));  
  XRCCTRL(*this, "txtCorrespondentAccount", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, 
                                                     "correspondent_account"),
                      ContraptionSetter<std::string>(contraption_, 
                                                     "correspondent_account")));  
  XRCCTRL(*this, "txtBIC", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "bic"),
                      ContraptionSetter<std::string>(contraption_, "bic")));  
  XRCCTRL(*this, "txtOKONH", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "okonh"),
                      ContraptionSetter<std::string>(contraption_, "okonh")));  

  XRCCTRL(*this, "txtRegisteredOffice", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, 
                                                     "registered_office"),
                      ContraptionSetter<std::string>(contraption_, 
                                                     "registered_office")));  
  XRCCTRL(*this, "txtAddress", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "address"),
                      ContraptionSetter<std::string>(contraption_, "address")));  
  XRCCTRL(*this, "txtOKONH", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "okonh"),
                      ContraptionSetter<std::string>(contraption_, "okonh")));  
  XRCCTRL(*this, "txtEmail", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "email"),
                      DummySetter<std::string>()));
  XRCCTRL(*this, "txtPhone", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "phone"),
                      DummySetter<std::string>()));
  XRCCTRL(*this, "txtNote", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "note"),
                      ContraptionSetter<std::string>(contraption_, "note")));  

  ContraptionChoice *choice_contact_person = 
      new ContraptionChoice(this, 
                            boost::bind(
                                static_cast
                                <std::string (Contraption::*)(const std::string&)>(
                                    &Contraption::Get<std::string>),
                                _1, std::string("name")));
  choice_contact_person->SetValidator(
      HasOneValidator(ContraptionGetter<ContraptionP>(contraption_, "contact_person"),
                      ContraptionSetter<ContraptionP>(contraption_, "contact_person"),
                      boost::bind(&DlgCompany::ContactPersons, this),
                      boost::bind(&DlgCompany::ContactPersonFactory, this)));  

  choice_contact_person->set_contraption_dialog(FramesCollection::dlg_contact_person);
  wxXmlResource::Get()->AttachUnknownControl("cchContactPerson", choice_contact_person);
  
  ContraptionGrid *grid_staff = new ContraptionGrid(this, wxID_ANY);
  
  std::vector<Column> cols;
  ModelP model = ContactPerson::GetModel();
  cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));
  cols.push_back(Column(model->GetFieldID("position"), "Должность", 100));
  cols.push_back(Column(model->GetFieldID("email"), "Email", 100));
  cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 100));
  ContraptionGridTableBase *table_staff =
      new ContraptionGridTableBase(model->Empty(), cols);
  grid_staff->SetValidator(HasManyValidator(
      ContraptionGetter<ContraptionArrayP>(contraption_, "contact_persons"),
      boost::bind(&DlgCompany::ContactPersonFactory, this)));

  table_staff->set_contraption_dialog(FramesCollection::dlg_contact_person);
  grid_staff->SetTable(table_staff);
  grid_staff->set_add_button(XRCCTRL(*this, "btnAddStaff", wxButton));
  wxXmlResource::Get()->AttachUnknownControl("cgrStaff", grid_staff);
  SetSaveButton(XRCCTRL(*this, "btnSave", wxButton));
  SetDeleteButton(XRCCTRL(*this, "btnDelete", wxButton));
  SetCancelButton(XRCCTRL(*this, "btnCancel", wxButton));
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgCompany Initialized"));
}

void DlgCompany::Delete() {
  contraption_->Refresh();
  ContraptionArrayP contact_persons = 
      contraption_->Get<ContraptionArrayP>("contact_persons");
  while (contact_persons->size()) {
    contact_persons->erase(0);
  }
  contraption_->Save();
  ContraptionDialog::Delete();
}

}
}
