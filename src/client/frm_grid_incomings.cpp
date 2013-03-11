#include "frm_grid.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// std
#include "options.hpp"
// frames
#include "incomings_frame.hpp"
#include "dlg_incoming.hpp"
#include "frames_collection.hpp"

namespace tms{
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget;
using namespace tms::project::model;
using namespace tms::common::model;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;

void FrmGrid::InitIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Incomings table"));
  std::vector<Column> cols;
  ModelP model = Incoming::GetModel();
  cols.push_back(Column(model->GetFieldID("ID"), "ID", 150));
  cols.push_back(Column(model->GetFieldID("company_name"), "Компания", 100));
  cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 150));
  cols.push_back(Column(model->GetFieldID("email"), "E-mail", 150));
  cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 150));
  cols.push_back(Column(model->GetFieldID("subject"), "Тема", 150));
  cols.push_back(Column(model->GetFieldID("addressee"), "Адресат", 100));
  cols.push_back(Column(model->GetFieldID("reception_type"), "Тип приема", 100));
  cols.push_back(Column(model->GetFieldID("received_at"), "Время приема", 100));
  cols.push_back(Column(model->GetFieldID("passed_at"), "Время передачи", 100));
  table_incomings_ =
      new ContraptionGridTableBase(model->All(), "Вxодящие", cols);
  table_incomings_->set_contraption_dialog(FramesCollection::dlg_incoming);
}

}
}
