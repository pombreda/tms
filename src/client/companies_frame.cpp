#include "companies_frame.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/notebook.h>
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// common
#include <protocol/crypto.hpp>
// frames
#include "frames_collection.hpp"
#include "frm_grid.hpp"
// models
#include <project/model/contact_person.hpp>
// boost
#include <boost/lexical_cast.hpp>
namespace tms {
namespace client {

using namespace boost;
using namespace tms::common::contraption;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::project::model;
using namespace tms::common::protocol;
using namespace tms::common::widget;
BEGIN_EVENT_TABLE(CompaniesFrame,wxFrame)
END_EVENT_TABLE()

CompaniesFrame::~CompaniesFrame() {
}

void CompaniesFrame::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing CompaniesFrame"));
  dlg_contact_person_ = new DlgContactPerson(this);  
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&CompaniesFrame::OnSaveClick);
  Connect(XRCID("ID_BUTTONREMOVE"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&CompaniesFrame::OnDeleteClick);
  Connect(XRCID("ID_BUTTONCANCEL"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&CompaniesFrame::OnExitClick);
  grid_contact_persons_ = new ContraptionGrid(this, wxID_ANY);
      wxXmlResource::Get()->AttachUnknownControl("ID_GRID_CONTACT_PERSON", (wxWindow *)grid_contact_persons_);
  FindWindowByName("ID_PANEL3", this)->Fit();
  FindWindowByName("ID_PANEL3", this)->Layout();


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
  button_cancel_ = (wxButton*)FindWindowByName("ID_BUTTONCANCEL", this);

  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Other controls loaded"));   

  // Set up  
  LOG4CPLUS_DEBUG(client_logger, 
		 WStringFromUTF8String("Setting up values"));       

  button_remove_->Show(true);
  bool is_new = contraption->IsNew();
  if (is_new) {
    button_remove_->Show(false);
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
    contraption->Set<std::string>("ID", id);
    contraption->Save();
    button_cancel_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
    button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
			    (wxObjectEventFunction)&CompaniesFrame::OnDeleteClick, 0, this);
  } else {
    button_cancel_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
    button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
			    (wxObjectEventFunction)&CompaniesFrame::OnExitClick, 0, this);
  }
  // personal
  choice_contact_person_ = (wxChoice*)FindWindowByName("ID_CHOICECONTACT_PERSON", this);
  button_add_in_contact_person_ = (wxButton*)FindWindowByName("ID_BUTTONADD_CONTACT_PERSON", this);
  button_add_in_contact_person_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
					 (wxObjectEventFunction)&CompaniesFrame::OnAddInContactPersonClick, 0, this);
  std::vector<Column> cols;
  contact_persons_ = contraption->Get<ContraptionArrayP>("contact_persons");
  InitChoiceContactPersson(contraption->Get<int>("contact_person_id"));
  ModelP model = contact_persons_->model();
  cols.push_back(Column(model->GetFieldID("name"), "Имя", 100));
  cols.push_back(Column(model->GetFieldID("email"), "Email", 100));
  cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 50));
  table_contact_persons_ =
    new ContraptionGridTableBase(contact_persons_, cols);
  grid_contact_persons_->SetTable(table_contact_persons_, wxGrid::wxGridSelectRows, 2500);
  grid_contact_persons_->SetOnCellClick(boost::bind(&CompaniesFrame::OnContactPersonsCellClick,
						    this, _1, _2));
  grid_contact_persons_->SetOnCellDClick(boost::bind(&CompaniesFrame::OnContactPersonsCellDClick,
						     this, _1, _2));  
  grid_contact_persons_->Fit();
  grid_contact_persons_->Layout();
  Fit();
  Layout();
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("Personal controls loaded"));   

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

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
  Fit();
  Layout();
  FindWindowByName("ID_PANEL3", this)->Layout();

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
  // personal
  if (choice_contact_person_->GetSelection() >= 0) {
    contraption_->Set<int>("contact_person_id", contact_persons_id_[choice_contact_person_->GetSelection()]);
  }
  // contacts
  contraption_->Set<std::string>("yur_addr", tc_yur_addr_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("fact_addr", tc_fact_addr_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("mail", tc_mail_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("phone", tc_phone_->GetValue().utf8_str().data());
  // other
  contraption_->Set<std::string>("comment", tc_comment_->GetValue().utf8_str().data());

  contraption_->Save();
  contraptions_->Refresh();
  FramesCollection::frm_grid->Refresh();
  Hide();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
}

void CompaniesFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contact_persons_->Refresh();
  for (size_t i = 0; i < contact_persons_->size(); ++i) {
      contact_persons_->at(i)->Delete();
  }
  contraption_->Delete();
  contraptions_->Refresh();
  FramesCollection::frm_grid->Refresh();
  Hide();
}

void CompaniesFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void CompaniesFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  wxCloseEvent new_event(wxEVT_COMMAND_BUTTON_CLICKED);
  button_cancel_->ProcessWindowEvent(new_event);
  Hide();
}

void CompaniesFrame::InitChoiceContactPersson(int contact_person_id) {
  int selection = wxNOT_FOUND;
  choice_contact_person_->Clear();
  contact_persons_id_.clear();
  for (size_t i = 0; i < contact_persons_->size(); ++i) {
    LOG4CPLUS_DEBUG(client_logger, 
		    WStringFromUTF8String("Controls loaded"));
    
    ContraptionP contact_person = contact_persons_->at(i);
    choice_contact_person_->Append(wxString::FromUTF8(contact_person->Get<std::string>("name").c_str()));
    contact_persons_id_.push_back(contact_person->Get<int>("id"));
    if (contact_persons_id_.back() == contact_person_id) { 
      selection = i;
    }
  }
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("company is " + boost::lexical_cast<std::string>(selection)));

  choice_contact_person_->SetSelection(selection);  

}

void CompaniesFrame::OnContactPersonsCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void CompaniesFrame::OnContactPersonsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnContactPersonsCellDClick"));
  cur_contact_person_id_ = -1;
  if (choice_contact_person_->GetSelection() != wxNOT_FOUND
      &&
      static_cast<size_t>(choice_contact_person_->GetSelection()) < contact_persons_id_.size()) {
    cur_contact_person_id_ = contact_persons_id_[choice_contact_person_->GetSelection()];
  }
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_contact_persons_->GetTable())->
    contraptions();
  dlg_contact_person_->SetUpValues(contraption, contraptions, true);
  dlg_contact_person_->Show();
  dlg_contact_person_->ShowModal();
  Refresh();
  InitChoiceContactPersson(cur_contact_person_id_);
}

void CompaniesFrame::OnAddInContactPersonClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInContactPersonClick"));
  try {
    if (contraption_) {   
      cur_contact_person_id_ = -1;
      if (choice_contact_person_->GetSelection() != wxNOT_FOUND
          &&
          static_cast<size_t>(choice_contact_person_->GetSelection()) < contact_persons_id_.size()) {
	cur_contact_person_id_ = contact_persons_id_[choice_contact_person_->GetSelection()];
      }                                                                                                    
      ContraptionArrayP contraptions =
	dynamic_cast<ContraptionGridTableBase*>(grid_contact_persons_->GetTable())->contraptions();
      ContraptionP contraption = contraptions->model()->New();
      contraption->Set<int>("company_id", contraption_->Get<int>("id"));
      dlg_contact_person_->SetUpValues(contraption, contraptions, true);
      dlg_contact_person_->ShowModal();
      Refresh();
      InitChoiceContactPersson(cur_contact_person_id_);
    }
  } catch (GUIException &e) {
    Report(e);
  }
}

void CompaniesFrame::OnTimerContactPerson(wxTimerEvent& WXUNUSED(event))  {
  if (!FramesCollection::contact_persons_frame->IsVisible()) {
    timer_->Stop();
    Enable(true);
    Refresh();
    InitChoiceContactPersson(cur_contact_person_id_);
  }
}


}
}
