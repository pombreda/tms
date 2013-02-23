#include "incomings_frame.hpp"
#include "grid_frame.hpp"
#include "frames_collection.hpp"
#include "options.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/utils.h>
#include <wx/timer.h>

// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
#include <protocol/crypto.hpp>
// models
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <project/model/company.hpp>
#include <model/user.hpp>
// boost
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/lexical_cast.hpp>
// std
#include <sstream>

namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::model;
using namespace tms::common::protocol;
using namespace tms::project::model;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace std;
using namespace boost;

BEGIN_EVENT_TABLE(IncomingsFrame,wxFrame)
END_EVENT_TABLE()

IncomingsFrame::~IncomingsFrame() {
}

void IncomingsFrame::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing IncomingsFrame"));
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnSaveClick);
  Connect(XRCID("ID_BUTTONREMOVE"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnDeleteClick);
  Connect(XRCID("ID_BUTTONSEND"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnSendClick);
  Connect(XRCID("ID_BUTTONADD_COMPANY"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnAddCompanyClick);
  Connect(XRCID("ID_CHOICECOMPANY"), wxEVT_COMMAND_CHOICE_SELECTED,
          (wxObjectEventFunction)&IncomingsFrame::OnCompanyChoice);

  Bind(wxEVT_CLOSE_WINDOW, &IncomingsFrame::OnTryClose, this);
                                            

  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("IncomingsFrame Initialized"));
}

void IncomingsFrame::SetUpValues(ContraptionP contraption,
                            ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Setting up IncomingsFrame values"));
  contraption_ = contraption;
  contraptions_ = contraptions;
  ModelP user_model = User::GetModel();
  const SimpleFieldT<int> *is_manager
      = dynamic_cast<const SimpleFieldT<int>*>(
          user_model->GetField("manager"));

  users_ = user_model->All();
  // Load controls
  button_send_ = (wxButton*)FindWindowByName("ID_BUTTONSEND", this);
  button_remove_ = (wxButton*)FindWindowByName("ID_BUTTONREMOVE", this);
  button_save_ = (wxButton*)FindWindowByName("ID_BUTTON1", this);
  button_cancel_ = (wxButton*)FindWindowByName("ID_BUTTONCANCEL", this);
  button_add_company_ = (wxButton*)FindWindowByName("ID_BUTTONADD_COMPANY", this);
  button_add_contact_person_ = (wxButton*)FindWindowByName("ID_BUTTONADD_CONTACT_PERSON", this);
  tc_id_ = (wxTextCtrl*)FindWindowByName("ID_TEXTID", this);
  tc_time_in_ = (wxTextCtrl*)FindWindowByName("ID_TEXTTIME_IN", this);
  tc_time_out_ = (wxTextCtrl*)FindWindowByName("ID_TEXTTIME_OUT", this);
  tc_topic_ = (wxTextCtrl*)FindWindowByName("ID_TEXTTOPIC", this);  
  choice_type_in_ = (wxChoice*)FindWindowByName("ID_CHOICETYPE_IN", this);  
  choice_company_ = (wxChoice*)FindWindowByName("ID_CHOICECOMPANY", this);  
  choice_manager_ = (wxChoice*)FindWindowByName("ID_CHOICEMANAGER", this);  
  choice_contact_person_ = (wxChoice*)FindWindowByName("ID_CHOICECONTACT_PERSON", this);  

  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("Controls loaded"));
  // Set up  
  button_remove_->Show(true);
  button_save_->Show(true);
  bool is_new = contraption->IsNew();
  if (is_new) {
    button_remove_->Show(false);
    contraption->Save();
    button_cancel_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
    button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
			    (wxObjectEventFunction)&IncomingsFrame::OnDeleteClick, 0, this);
  } else {
    button_cancel_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
    button_cancel_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
			    (wxObjectEventFunction)&IncomingsFrame::OnExitClick, 0, this);
  }
  if (!Options::secretair() || contraption->Get<std::string>("time_out") != "") {
    button_remove_->Show(false);
    button_save_->Show(false);
  }
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("Button set up"));

  if (is_new) {
    ptime now = second_clock::local_time();
    contraptions_->Refresh();
    time_facet *facet = new time_facet("%y");
    ostringstream sout;
    sout.imbue(locale(cout.getloc(), facet));
    sout << now;
    std::string year = sout.str();
    
    std::string id ="0";
    if (contraptions_->size() > 0) {
      int p = contraptions_->size() - 1;
      while (p >= 0 && contraptions_->at(p)->Get<std::string>("ID") == "") {
	--p;
      }
      if (p >= 0) {
	ContraptionP last = contraptions_->at(p);
	std::string last_id = last->Get<std::string>("ID");
	size_t found = last_id.find("/Вх-");
	if (found != std::string::npos) {
	  try {
	    id = lexical_cast<std::string>(1 + lexical_cast<int>(last_id.substr(0, found)));
	  } catch (...) {
	  }
	}
      }
    }

    contraption->Set<std::string>("ID", id + "/Вх-" + year);
    contraption->Save();
    contraptions_->Refresh();
  }
  contact_persons_.reset();
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("ID generated"));
  companies_ = Company::GetModel()->All();

  choice_company_->Clear();
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("Reading companies"));
  ContraptionArrayP companies = contraption_->Get<ContraptionArrayP>("company");
  if (companies->size() > 0) {
    contact_persons_ = companies->at(0)->Get<ContraptionArrayP>("contact_persons");
  }
  InitChoiceCompany(contraption_->Get<int>("company_id"));
  InitChoiceManager(contraption_->Get<int>("manager_id"));
  InitChoiceContactPerson(contraption_->Get<int>("contact_person_id"));
  tc_id_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("ID").c_str()));
  std::string time_in = contraption_->Get<std::string>("time_in");
  if (time_in == "") {
    ptime now = second_clock::local_time();
    time_facet *facet = new time_facet("%Y-%m-%d %H:%M");
    ostringstream sout;
    sout.imbue(locale(cout.getloc(), facet));
    sout << now;
    time_in = sout.str();
  }
  tc_time_in_->ChangeValue(wxString::FromUTF8(time_in.c_str()));
  tc_topic_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("topic").c_str()));
  std::string time_out = contraption_->Get<std::string>("time_out");
  tc_time_out_->ChangeValue(wxString::FromUTF8(time_out.c_str()));
  if (time_out != "") {
    button_send_->Show(false);
  } else {
    button_send_->Show(true);
  }  
  std::string type_in = contraption_->Get<std::string>("type_in");
  choice_type_in_->SetStringSelection(wxString::FromUTF8(type_in.c_str()));
  if (type_in == "") {
    choice_type_in_->SetSelection(wxNOT_FOUND);
  }
  Fit();
  Layout();  
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
}

void IncomingsFrame::OnSendClick(wxCommandEvent& WXUNUSED(event)) {
  ptime now = second_clock::local_time();
  time_facet *facet = new time_facet("%Y-%m-%d %H:%M");
  ostringstream sout;
  sout.imbue(locale(cout.getloc(), facet));
  sout << now;
  std::string time_out = sout.str();
  tc_time_out_->ChangeValue(wxString::FromUTF8(time_out.c_str()));
  button_send_->Show(false);  
  Fit();
  Layout();
}

void IncomingsFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving values"));

  contraption_->Set<std::string>("ID", tc_id_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("time_in", tc_time_in_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("time_out", tc_time_out_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("topic", tc_topic_->GetValue().utf8_str().data());
  contraption_->Set<std::string>("type_in", choice_type_in_->GetStringSelection().utf8_str().data());
  try {
    if (choice_company_->GetSelection() >= 0) {
      LOG4CPLUS_INFO(client_logger, 
		     WStringFromUTF8String("Saving company"));
      LOG4CPLUS_DEBUG(client_logger, 
		      WStringFromUTF8String("company is " + boost::lexical_cast<std::string>(choice_company_->GetSelection()) + " or " + companies_->at(choice_company_->GetSelection())->Get<std::string>("short_name")));
      LOG4CPLUS_DEBUG(client_logger, 
		      WStringFromUTF8String("company id is " + boost::lexical_cast<std::string>( companies_->at(choice_company_->GetSelection())->Get<int>("id"))));

      contraption_->Set<int>("company_id", company_->Get<int>("id"));
    } else {
      wxMessageDialog *msg = new wxMessageDialog(this, wxString::FromUTF8("Необходимо указать компанию"), wxString::FromUTF8("Ошибка"));
      msg->ShowModal();
      
      return;
    }
    if (choice_manager_->GetSelection() >= 0) {
      contraption_->Set<int>("manager_id", users_->at(choice_manager_->GetSelection())->Get<int>("id"));
    }
  } catch (GUIException &e) {
    Report(e);
  }

  if (choice_contact_person_->GetSelection() >= 0) {   
    contraption_->Set<int>("contact_person_id", contact_persons_->at(choice_contact_person_->GetSelection())->Get<int>("id"));
  }
  LOG4CPLUS_INFO(client_logger, 
		 WStringFromUTF8String("Company saved"));

  contraption_->Save();
  contraptions_->Refresh();
  
  Hide();
  FramesCollection::grid_frame->Refresh();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
}

void IncomingsFrame::InitChoiceCompany(int company_id) {
  int selection = wxNOT_FOUND;
  choice_company_->Clear();
  for (size_t i = 0; i < companies_->size(); ++i) {
    LOG4CPLUS_DEBUG(client_logger, 
		    WStringFromUTF8String("Init choice company"));
    
    ContraptionP company = companies_->at(i);
    choice_company_->Append(wxString::FromUTF8(company->Get<std::string>("short_name").c_str()));
    if (company->Get<int>("id") == company_id) { 
      selection = i;
    }
  }
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("company is " + boost::lexical_cast<std::string>(selection)));

  choice_company_->SetSelection(selection);  
}

void IncomingsFrame::InitChoiceContactPerson(int contact_person_id) {
  int selection = wxNOT_FOUND;
  choice_contact_person_->Clear();
  contact_persons_id_.clear();
  if (contact_persons_) {
    for (size_t i = 0; i < contact_persons_->size(); ++i) {
      LOG4CPLUS_DEBUG(client_logger, 
		      WStringFromUTF8String("Init choice contact_persons"));
      
      ContraptionP contact_person = contact_persons_->at(i);
      choice_contact_person_->Append(wxString::FromUTF8(contact_person->Get<std::string>("name").c_str()));
      contact_persons_id_.push_back(contact_person->Get<int>("id"));
      if (contact_persons_id_.back() == contact_person_id) { 
	selection = i;
      }
    }
  }
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("contact_person is " + boost::lexical_cast<std::string>(selection)));

  choice_contact_person_->SetSelection(selection);  
}

void IncomingsFrame::InitChoiceManager(int manager_id) {
  try {
    int selection = wxNOT_FOUND;
    choice_manager_->Clear();
    for (size_t i = 0; i < users_->size(); ++i) {
      LOG4CPLUS_DEBUG(client_logger, 
		      WStringFromUTF8String("Init choice Manager"));
      
      ContraptionP user = users_->at(i);
      choice_manager_->Append(wxString::FromUTF8(user->Get<std::string>("name").c_str()));
      if (user->Get<int>("id") == manager_id) { 
	selection = i;
      }
    }
    LOG4CPLUS_DEBUG(client_logger, 
		    WStringFromUTF8String("manager is " + boost::lexical_cast<std::string>(selection)));
    
    choice_manager_->SetSelection(selection);  

  } catch (GUIException &e) {
    Report(e);
  }
  
}

void IncomingsFrame::OnAddCompanyClick(wxCommandEvent& WXUNUSED(event)) {
  company_ = companies_->model()->New();
  FramesCollection::companies_frame->SetUpValues(company_, companies_);
  FramesCollection::companies_frame->Show(true);
  Enable(false);
  Disconnect(wxEVT_TIMER);
  Connect(wxEVT_TIMER, (wxObjectEventFunction)&IncomingsFrame::OnAddCompanyTimer);  
  add_company_timer_->Start(300);
}

void IncomingsFrame::OnCompanyChoice(wxCommandEvent& WXUNUSED(event)) {
  company_ = companies_->at(choice_company_->GetSelection());
  int contact_person_id = -1;
  if (choice_contact_person_->GetSelection() > 0) {
    contact_person_id = contact_persons_->at(choice_contact_person_->GetSelection())->Get<int>("ID");
  }
  contact_persons_ = company_->Get<ContraptionArrayP>("contact_persons");
  InitChoiceContactPerson(contact_person_id);  
}

void IncomingsFrame::OnAddContactPersonClick(wxCommandEvent& WXUNUSED(event)) {
  contact_person_ = contact_persons_->model()->New();
  FramesCollection::contact_persons_frame->SetUpValues(company_, companies_, true);
  FramesCollection::contact_persons_frame->Show(true);
  Enable(false);
  Disconnect(wxEVT_TIMER);
  Connect(wxEVT_TIMER, (wxObjectEventFunction)&IncomingsFrame::OnAddContactPersonTimer);  
  add_company_timer_->Start(300);

}

void IncomingsFrame::OnAddCompanyTimer(wxTimerEvent& WXUNUSED(event))  {
  if (!FramesCollection::companies_frame->IsVisible()) {
    add_company_timer_->Stop();
    Enable(true);
    if (!company_->IsNew()) {
      InitChoiceCompany(company_->Get<int>("id"));
      company_ = companies_->at(choice_company_->GetSelection());
      int contact_person_id = -1;
      if (choice_contact_person_->GetSelection() > 0) {
	contact_person_id = contact_persons_->at(choice_contact_person_->GetSelection())->Get<int>("ID");
      }
      contact_persons_ = company_->Get<ContraptionArrayP>("contact_persons");
      InitChoiceContactPerson(contact_person_id);
    }
  }
}

void IncomingsFrame::OnAddContactPersonTimer(wxTimerEvent& WXUNUSED(event))  {
  if (!FramesCollection::contact_persons_frame->IsVisible()) {
    add_company_timer_->Stop();
    Enable(true);
    if (!contact_person_->IsNew()) {
      InitChoiceContactPerson(contact_person_->Get<int>("id"));
    }
  }
}


void IncomingsFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  FramesCollection::grid_frame->Refresh();
  Hide();
}

void IncomingsFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void IncomingsFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  wxCloseEvent new_event(wxEVT_COMMAND_BUTTON_CLICKED);
  button_cancel_->ProcessWindowEvent(new_event);
}

}

}
