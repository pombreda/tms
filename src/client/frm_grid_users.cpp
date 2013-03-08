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
#include "dlg_user.hpp"
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

void FrmGrid::InitUsersTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Users table"));
  std::vector<Column> cols;
  ModelP model = User::GetModel();
  cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));

  table_users_ =
      new ContraptionGridTableBase(model->All(), "Пользователи", cols);
  table_users_->set_contraption_dialog(FramesCollection::dlg_user);
}

}
}
