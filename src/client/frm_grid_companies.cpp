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
//#include "companies_frame.hpp"
#include "frames_collection.hpp"
#include "dlg_company.hpp"

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

void FrmGrid::InitCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Companies table"));
  std::vector<Column> cols;
  ModelP model = Company::GetModel();
  cols.push_back(Column(model->GetFieldID("ID"), "ID", 30));
  cols.push_back(Column(model->GetFieldID("short_name"), "Название", 150));
  cols.push_back(Column(model->GetFieldID("group"), "Группа", 100));
  cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 120));
  cols.push_back(Column(model->GetFieldID("email"), "E-mail", 130));
  cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 150));
  table_companies_ =
      new ContraptionGridTableBase(model->All(), "Компании", cols);
  table_companies_->set_contraption_dialog(FramesCollection::dlg_company);
}

}
}
