#include "frm_grid.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// project
#include <project/model/addressee.hpp>
// std
#include "options.hpp"
// frames
#include "dlg_addressee.hpp"
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

void FrmGrid::InitAddresseesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Addressees table"));
  std::vector<Column> cols;
  ModelP model = Addressee::GetModel();
  cols.push_back(Column(model->GetFieldID("addressee"), "Адресаты", 150));

  table_addressees_ =
      new ContraptionGridTableBase(model->All(), "Адресаты", cols);
  table_addressees_->set_contraption_dialog(FramesCollection::dlg_addressee);
}

}
}
