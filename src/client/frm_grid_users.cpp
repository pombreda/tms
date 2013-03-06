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
    new ContraptionGridTableBase(model->All(), cols);

}

void FrmGrid::ActivateUsersTable() {
  grid_admin_->ResetColPos();
  grid_admin_->SetTable(table_users_, wxGrid::wxGridSelectRows, 2500);
  button_add_in_admin_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
  button_add_in_admin_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                       (wxObjectEventFunction)&FrmGrid::OnAddInUserClick,
                       0, this);
  grid_admin_->SetOnCellDClick(boost::bind(&FrmGrid::OnUsersCellDClick, this, _1, _2));
}

void FrmGrid::OnUsersCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAdminCellDClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    FramesCollection::dlg_user->SetUpValues(contraption, contraptions);
    FramesCollection::dlg_user->ShowModal();
  } catch (GUIException &e) {
    Report(e);
  }
}

void FrmGrid::OnAddInUserClick(wxCommandEvent& WXUNUSED(event)) {
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_admin_->GetTable())->
    contraptions();
  ContraptionP contraption = contraptions->model()->New();
  FramesCollection::dlg_user->SetUpValues(contraption, contraptions);
  FramesCollection::dlg_user->ShowModal();
}

}
}
