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
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Values set up"));
}

void CompaniesFrame::OnSaveClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving values"));
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
