#include "contraption_dialog.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <rtti/typeinfo.hpp>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;
using namespace string;
using namespace log4cplus;

ContraptionDialog::ContraptionDialog() :
    wxDialog(),
    logger_(Logger::getInstance(WStringFromUTF8String(rtti::TypeID(this).name()))) {}

ContraptionDialog::~ContraptionDialog() {}

void ContraptionDialog::SetUpValues(ContraptionP contraption,
                                    ContraptionArrayP contraptions) {
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Loading contraption"));
  contraption_ = contraption;
  
  contraptions_ = contraptions;
  TransferDataToWindow();
  
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
  LOG4CPLUS_INFO(logger_, 
                 WStringFromUTF8String("Closing"));
  EndModal(wxCANCEL);  
}

}
}
}
