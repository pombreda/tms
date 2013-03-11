#include "contraption_dialog.hpp"
// log4cplus
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <widget/validators/contraption_routines.hpp>
#include <widget/validators/hide_if_validator.hpp>
#include <rtti/typeinfo.hpp>
// wx
#include <wx/button.h>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;
using namespace string;
using namespace log4cplus;
using namespace validators;

ContraptionDialog::ContraptionDialog() :
    wxDialog(),
    logger_(Logger::getInstance(WStringFromUTF8String(rtti::TypeID(this).name()))) {
  Bind(wxEVT_CLOSE_WINDOW, boost::bind(&ContraptionDialog::Exit, this));
}

ContraptionDialog::~ContraptionDialog() {}

void ContraptionDialog::SetUpValues(ContraptionP contraption,
                                    ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Loading contraption"));
  contraption_ = contraption;
  
  contraptions_ = contraptions;
  is_new_ = false;
  TransferDataToWindow();
  if (static_cast<size_t>(contraption_->Get<int>("id")) == Contraption::kNewID) {
    TransferDataFromWindow();
    is_new_ = true;
    contraption_->Save();
    contraption_->Refresh();
    TransferDataToWindow();
  }
  Fit();
  Layout();
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Contraption loaded"));
}

void ContraptionDialog::Save() {
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Saving contraption"));
  
  if (!Validate() || !TransferDataFromWindow()) {
    return;
  }
  contraption_->Save();
  contraptions_->Refresh();
  
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Contraption saved. Closing"));
  EndModal(wxOK);
}

void ContraptionDialog::Delete() {
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Deleting conraption"));

  contraption_->Delete();
  contraptions_->Refresh();
  
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Contraption deleted. Closing"));
  EndModal(wxOK);  
}

void ContraptionDialog::Exit() {
  if (is_new_) {
    Delete();
  } else {
    LOG4CPLUS_INFO(logger_, 
                   WStringFromUTF8String("Closing"));
    EndModal(wxCANCEL);  
  }
}


void ContraptionDialog::SetSaveButton(wxButton *btn_save) {
  btn_save->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                 boost::bind(&ContraptionDialog::Save, this));
}

void ContraptionDialog::SetCancelButton(wxButton *btn_cancel) {
  btn_cancel->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                   boost::bind(&ContraptionDialog::Exit, this));
}

void ContraptionDialog::SetDeleteButton(wxButton *btn_delete) {
  btn_delete->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                   boost::bind(&ContraptionDialog::Delete, this));
  btn_delete->SetValidator(
      HideIfValidator(DefaultGetter<bool>(
          ContraptionIsNew(contraption_),
          VarRefGetter<bool>(is_new_))));
}

}
}
}
