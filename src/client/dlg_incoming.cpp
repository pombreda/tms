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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/lexical_cast.hpp>
// common
#include <protocol/crypto.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/string_validator.hpp>
#include <widget/validators/bool_validator.hpp>
#include <widget/validators/has_one_validator.hpp>
#include <widget/validators/password_validator.hpp>
#include <widget/validators/hide_if_validator.hpp>
#include <widget/contraption_choice.hpp>
// project
#include <project/model/company.hpp>
#include <project/model/contact_person.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"
#include "dlg_company.hpp"
#include "dlg_contact_person.hpp"

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

namespace tms {
namespace client {

using namespace common::contraption;
using namespace project::model;
using namespace common::widget;
using namespace common::widget::validators;
using namespace common::string;
using namespace common;
using namespace common::protocol;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace boost;
DlgIncoming::~DlgIncoming() {
}

DlgIncoming::DlgIncoming(wxWindow *parent) :
    ContraptionDialog() {
  if (!wxXmlResource::Get()->LoadDialog(this, parent,
                                        _T("dlgIncoming"))) {
    LOG4CPLUS_ERROR(logger_, 
                    WStringFromUTF8String("Can't load dialog"));    
  }
  Init();
}

std::string DlgIncoming::CurTime() {
  ptime now = second_clock::local_time();
  time_facet *facet = new time_facet("%Y-%m-%d %H:%M");
  ostringstream sout;
  sout.imbue(locale(cout.getloc(), facet));
  sout << now;
  return sout.str();
}

bool DlgIncoming::Passed() {
  return contraption_->Get<std::string>("passed_at") != std::string();
}

void DlgIncoming::OnCompanyChange(ContraptionP /*contraption*/) {
  ContraptionP company = choice_company_->GetSelection();
  choice_contact_person_->GetValidator()->TransferToWindow();
}

ContraptionArrayP DlgIncoming::ContactPersons() {
  ContraptionP company = choice_company_->GetSelection();
  if (!company) {
    return ContraptionArrayP();
  }
  wxASSERT(company->model() == Company::GetModel());
  return company->Get<ContraptionArrayP>("contact_persons");
}

ContraptionArrayP DlgIncoming::Companies() {
  return Company::GetModel()->All();
}

ContraptionP DlgIncoming::ContactPersonFactory() {
  ContraptionP contact_person = ContactPerson::GetModel()->New();
  ContraptionP company = choice_company_->GetSelection();
  wxASSERT(company);
  wxASSERT(company->model() == Company::GetModel());
  contact_person->Set<int>("company_id", company->Get<int>("id"));
  return contact_person;
}

ContraptionP DlgIncoming::CompanyFactory() {
  return Company::GetModel()->New();
}


void DlgIncoming::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing DlgIncoming"));
  
  XRCCTRL(*this, "txtID", wxTextCtrl)->SetValidator(
      StringValidator(DefaultGetter<std::string>(
          ContraptionGetter<std::string>(contraption_, "ID"), 
          Incoming::GenerateID),
                      ContraptionSetter<std::string>(contraption_, "ID")));
  XRCCTRL(*this, "txtRecievedAt", wxTextCtrl)->SetValidator(
      StringValidator(DefaultGetter<std::string>(
          ContraptionGetter<std::string>(contraption_, "received_at"), 
          boost::bind(&DlgIncoming::CurTime, this)),
                      ContraptionSetter<std::string>(contraption_, "received_at")));
  XRCCTRL(*this, "chcReceptionType", wxChoice)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "reception_type"),
                      ContraptionSetter<std::string>(contraption_, 
                                                     "reception_type")));
  XRCCTRL(*this, "txtSubject", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "subject"),
                      ContraptionSetter<std::string>(contraption_, "subject")));  
  txt_passed_at_ = XRCCTRL(*this, "txtPassedAt", wxTextCtrl);
  txt_passed_at_->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "passed_at"),
                      ContraptionSetter<std::string>(contraption_, "passed_at")));  
  XRCCTRL(*this, "txtAddressee", wxTextCtrl)->SetValidator(
      StringValidator(ContraptionGetter<std::string>(contraption_, "addressee"),
                      ContraptionSetter<std::string>(contraption_, "addressee")));  
  
  choice_company_ = 
      new ContraptionChoice(this, 
                            boost::bind(
                                static_cast
                                <std::string (Contraption::*)(const std::string&)>(
                                    &Contraption::Get<std::string>),
                                _1, std::string("short_name")));
  choice_company_->SetValidator(
      HasOneValidator(ContraptionGetter<ContraptionP>(contraption_, "company"),
                      ContraptionSetter<ContraptionP>(contraption_, "company"),
                      boost::bind(&DlgIncoming::Companies, this),
                      boost::bind(&DlgIncoming::CompanyFactory, this)));  

  choice_company_->set_contraption_dialog(FramesCollection::dlg_company);
  choice_company_->BindOnChoice(boost::bind(&DlgIncoming::OnCompanyChange, 
                                            this, _1));

  wxXmlResource::Get()->AttachUnknownControl("cchCompany", choice_company_);

  choice_contact_person_ = 
      new ContraptionChoice(this, 
                            boost::bind(
                                static_cast
                                <std::string (Contraption::*)(const std::string&)>(
                                    &Contraption::Get<std::string>),
                                _1, std::string("name")));
  choice_contact_person_->SetValidator(
      HasOneValidator(ContraptionGetter<ContraptionP>(contraption_, "contact_person"),
                      ContraptionSetter<ContraptionP>(contraption_, "contact_person"),
                      boost::bind(&DlgIncoming::ContactPersons, this),
                      boost::bind(&DlgIncoming::ContactPersonFactory, this)));  

  choice_contact_person_->set_contraption_dialog(
      FramesCollection::dlg_contact_person);
  wxXmlResource::Get()->AttachUnknownControl("cchContactPerson", 
                                             choice_contact_person_);



  wxButton *btn_passed = XRCCTRL(*this, "btnPass", wxButton);
  btn_passed->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                   boost::bind(&DlgIncoming::Pass, this));
  btn_passed->SetValidator(
      HideIfValidator(boost::bind(&DlgIncoming::Passed, this)));
  SetSaveButton(XRCCTRL(*this, "btnSave", wxButton));
  SetDeleteButton(XRCCTRL(*this, "btnDelete", wxButton));
  SetCancelButton(XRCCTRL(*this, "btnCancel", wxButton));

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("DlgIncoming Initialized"));
}

void DlgIncoming::Pass() {
  if (txt_passed_at_->GetValue().utf8_str().data() == std::string()) {
    txt_passed_at_->SetValue(wxString::FromUTF8(CurTime().c_str()));
  }
}

}
}
