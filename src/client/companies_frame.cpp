#include "companies_frame.hpp"
// wx
#include <wx/msgdlg.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// common
#include <protocol/crypto.hpp>
namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;
BEGIN_EVENT_TABLE(CompaniesFrame,wxFrame)
END_EVENT_TABLE()

CompaniesFrame::~CompaniesFrame() {
}

void CompaniesFrame::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing CompaniesFrame"));
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&CompaniesFrame::OnSaveClick);
  Connect(XRCID("ID_BUTTONREMOVE"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&CompaniesFrame::OnDeleteClick);
  Connect(XRCID("ID_BUTTONCANCEL"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&CompaniesFrame::OnExitClick);
  Bind(wxEVT_CLOSE_WINDOW, &CompaniesFrame::OnTryClose, this);
                                            

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("CompaniesFrame Initialized"));
}

void CompaniesFrame::SetUpValues(ContraptionP contraption,
                            ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting up CompaniesFrame values"));
  contraption_ = contraption;
  contraptions_ = contraptions;
  // Load controls
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Loading controls"));   
  // general
  tc_id_ = (wxTextCtrl*)FindWindowByName("ID_TEXTID", this);
  tc_full_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTFULL_NAME", this);
  tc_short_name_ = (wxTextCtrl*)FindWindowByName("ID_TEXTSHORT_NAME", this);
  tc_gruppa_ = (wxTextCtrl*)FindWindowByName("ID_TEXTGRUPPA", this);
  tc_okonh_ = (wxTextCtrl*)FindWindowByName("ID_TEXTOKONH", this);
  tc_ogrn_ = (wxTextCtrl*)FindWindowByName("ID_TEXTOGRN", this);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("General controls loaded"));   
  //  bank
  tc_bank_ = (wxTextCtrl*)FindWindowByName("ID_TEXTBANK", this);
  tc_bank_city_ = (wxTextCtrl*)FindWindowByName("ID_TEXTBANK_CITY", this);
  tc_lits_schet_ = (wxTextCtrl*)FindWindowByName("ID_TEXTLITS_SCHET", this);
  tc_cor_schet_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCOR_SCHET", this);
  tc_inn_ = (wxTextCtrl*)FindWindowByName("ID_TEXTINN", this);
  tc_bik_ = (wxTextCtrl*)FindWindowByName("ID_TEXTBIK", this);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Bank controls loaded"));   
  // personal
  choice_gen_dir_ = (wxChoice*)FindWindowByName("ID_CHOICEGENDIR", this);
  choice_glavbuh_ = (wxChoice*)FindWindowByName("ID_CHOICEGLAVBUH", this);
  choice_contact_person_ = (wxChoice*)FindWindowByName("ID_CHOICECONTACT_PERSON", this);
  button_add_in_contact_persons_ = (wxButton*)FindWindowByName("ID_BUTTONADD_IN_CONTACT_PERSON");
  grid_contact_persons_ = new ContraptionGrid(this, wxID_ANY);
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_CONTACT_PERSON", (wxWindow *)grid_contact_persons_);
  std::vector<Column> cols;
  cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));
  cols.push_back(Column(model->GetFieldID("email"), "Email", 50));
  cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 100));
  table_contact_persons_ =
    new ContraptionGridTableBase(contraption_->Get<ContraptionArrayP>("contact_persons"), cols);
  
  // contacts
  tc_yur_addr_ = (wxTextCtrl*)FindWindowByName("ID_TEXTYUR_ADDR", this);
  tc_fact_addr_ = (wxTextCtrl*)FindWindowByName("ID_TEXTFACT_ADDR", this);
  tc_mail_ = (wxTextCtrl*)FindWindowByName("ID_TEXTMAIL", this);
  tc_phone_ = (wxTextCtrl*)FindWindowByName("ID_TEXTPHONE", this);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Contacts controls loaded"));   
  // other
  tc_comment_ = (wxTextCtrl*)FindWindowByName("ID_TEXTCOMMENT", this);
  button_remove_ = (wxButton*)FindWindowByName("ID_BUTTONREMOVE", this);

  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Other controls loaded"));   

  // Set up  
  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Setting up values"));       

  if (contraption->IsNew()) {
    button_remove_->Show(false);
  } else {
    button_remove_->Show(true);
  }

  // general
  tc_id_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("ID").c_str()));
  tc_full_name_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("full_name").c_str()));
  tc_short_name_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("short_name").c_str()));
  tc_gruppa_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("gruppa").c_str()));
  tc_okonh_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("okonh").c_str()));
  tc_ogrn_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("ogrn").c_str()));

  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("General values set"));       

  // bank
  tc_bank_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("bank").c_str()));
  tc_bank_city_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("bank_city").c_str()));
  tc_lits_schet_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("lits_schet").c_str()));
  tc_cor_schet_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("cor_schet").c_str()));
  tc_inn_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("inn").c_str()));
  tc_bik_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("bik").c_str()));

  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Bank values set"));       

  // contacts
  tc_yur_addr_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("yur_addr").c_str()));
  tc_fact_addr_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("fact_addr").c_str()));
  tc_mail_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("mail").c_str()));
  tc_phone_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("phone").c_str()));

  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Contacts values set"));       

  // other
  tc_comment_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("comment").c_str()));


  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Other values set"));       

  Layout();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
}

void CompaniesFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving values"));
  // general
  contraption_->Set<std::string>("ID", tc_id_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("full_name", tc_full_name_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("short_name", tc_short_name_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("gruppa", tc_gruppa_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("okonh", tc_okonh_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("ogrn", tc_ogrn_->GetValue().utf8_str().data());
  // bank
  contraption_->Set<std::string>("bank", tc_bank_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("bank_city", tc_bank_city_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("lits_schet", tc_lits_schet_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("cor_schet", tc_cor_schet_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("inn", tc_inn_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("bik", tc_bik_->GetValue().utf8_str().data());
  // contacts
  contraption_->Set<std::string>("yur_addr", tc_yur_addr_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("fact_addr", tc_fact_addr_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("mail", tc_mail_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("phone", tc_phone_->GetValue().utf8_str().data());
  // other
  contraption_->Set<std::string>("comment", tc_comment_->GetValue().utf8_str().data());

  contraption_->Save();
  contraptions_->Refresh();
  Hide();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
}

void CompaniesFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  Hide();
}

void CompaniesFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void CompaniesFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  Hide();
}

}
}
