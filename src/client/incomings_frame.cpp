#include "incomings_frame.hpp"
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
#include <project/model/company.hpp>
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
  Connect(XRCID("ID_BUTTONCANCEL"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnExitClick);
  Connect(XRCID("ID_BUTTONSEND"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnSendClick);
  Connect(XRCID("ID_BUTTONADD_COMPANY"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&IncomingsFrame::OnAddCompanyClick);
  Connect(wxEVT_TIMER, (wxObjectEventFunction)&IncomingsFrame::OnAddCompanyTimer);
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
  // Load controls
  button_send_ = (wxButton*)FindWindowByName("ID_BUTTONSEND", this);
  button_remove_ = (wxButton*)FindWindowByName("ID_BUTTONREMOVE", this);
  button_save_ = (wxButton*)FindWindowByName("ID_BUTTON1", this);
  button_add_company_ = (wxButton*)FindWindowByName("ID_BUTTONADD_COMPANY", this);
  button_add_contact_person_ = (wxButton*)FindWindowByName("ID_BUTTONADD_CONTACT_PERSON", this);
  tc_id_ = (wxTextCtrl*)FindWindowByName("ID_TEXTID", this);
  tc_time_in_ = (wxTextCtrl*)FindWindowByName("ID_TEXTTIME_IN", this);
  tc_time_out_ = (wxTextCtrl*)FindWindowByName("ID_TEXTTIME_OUT", this);
  tc_topic_ = (wxTextCtrl*)FindWindowByName("ID_TEXTTOPIC", this);  
  choice_type_in_ = (wxChoice*)FindWindowByName("ID_CHOICETYPE_IN", this);  
  choice_company_ = (wxChoice*)FindWindowByName("ID_CHOICECOMPANY", this);  
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("Controls loaded"));

  // Set up  
  companies_ = Company::GetModel()->All();

  choice_company_->Clear();
  LOG4CPLUS_DEBUG(client_logger, 
		  WStringFromUTF8String("Reading companies"));
  
  InitChoiceCompany(contraption_->Get<int>("company_id"));
  button_remove_->Show(true);
  button_save_->Show(true);
  if (contraption->IsNew()) {
    button_remove_->Show(false);
  }

  if (!Options::secretair()) {
    button_remove_->Show(false);
    button_save_->Show(false);
  }

  tc_id_->ChangeValue(wxString::FromUTF8(contraption_->Get<std::string>("ID").c_str()));
  std::string time_in = contraption_->Get<std::string>("time_in");
  if (time_in == "") {
    ptime now = second_clock::local_time();
    time_facet *facet = new time_facet("%Y-%m-%d %H:%M:%S");
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
  time_facet *facet = new time_facet("%Y-%m-%d %H:%M:%S");
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

      contraption_->Set<int>("company_id", companies_->at(choice_company_->GetSelection())->Get<int>("id"));
    } else {
      wxMessageDialog *msg = new wxMessageDialog(this, wxString::FromUTF8("Необходимо указать компанию"), wxString::FromUTF8("Ошибка"));
      msg->ShowModal();
      
      return;
    }
  } catch (GUIException &e) {
    Report(e);
  }
  LOG4CPLUS_INFO(client_logger, 
		 WStringFromUTF8String("Company saved"));

  contraption_->Save();
  contraptions_->Refresh();
  
  Hide();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values saved"));
}

void IncomingsFrame::InitChoiceCompany(int company_id) {
  int selection = wxNOT_FOUND;
  choice_company_->Clear();
  for (size_t i = 0; i < companies_->size(); ++i) {
    LOG4CPLUS_DEBUG(client_logger, 
		    WStringFromUTF8String("Controls loaded"));
    
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

void IncomingsFrame::OnAddCompanyClick(wxCommandEvent& WXUNUSED(event)) {
  company_ = companies_->model()->New();
  companies_frame_->SetUpValues(company_, companies_);
  companies_frame_->Show(true);
  Enable(false);
  add_company_timer_->Start(300);
}

void IncomingsFrame::OnAddCompanyTimer(wxTimerEvent& WXUNUSED(event))  {
  if (!companies_frame_->IsVisible()) {
    add_company_timer_->Stop();
    Enable(true);
    InitChoiceCompany(company_->Get<int>("id"));
  }
}

void IncomingsFrame::OnDeleteClick(wxCommandEvent& WXUNUSED(event)) {
  contraption_->Delete();
  contraptions_->Refresh();
  Hide();
}

void IncomingsFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Hide();
}

void IncomingsFrame::OnTryClose(wxCloseEvent& event) {
  event.Veto();
  Hide();
}

}

}
