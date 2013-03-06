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
#include "companies_frame.hpp"
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

void FrmGrid::InitCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Companies table"));
  try {
    std::vector<Column> cols;
    ModelP model = Company::GetModel();
    cols.push_back(Column(model->GetFieldID("ID"), "ID", 30));
    cols.push_back(Column(model->GetFieldID("short_name"), "Название", 150));
    cols.push_back(Column(model->GetFieldID("gruppa"), "Группа", 100));
    cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 120));
    cols.push_back(Column(model->GetFieldID("contact_email"), "Контактный e-mail", 130));
    cols.push_back(Column(model->GetFieldID("contact_phone"), "Контактный телефон", 150));
    table_companies_ =
      new ContraptionGridTableBase(model->All(), cols);

  } catch (GUIException &e) {
    Report(e);
  }
}

void FrmGrid::OnCompaniesCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnCompaniesCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  FramesCollection::companies_frame->SetUpValues(contraption, contraptions);
  FramesCollection::companies_frame->Show(true);  
}

void FrmGrid::ActivateCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating companies table"));  
  button_add_in_catalog_->Show(true);
  grid_catalogs_->ResetColPos();
  grid_catalogs_->SetTable(table_companies_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellDClick(boost::bind(&FrmGrid::OnCompaniesCellDClick,
					      this, _1, _2));  

  button_add_in_catalog_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
  button_add_in_catalog_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                  (wxObjectEventFunction)&FrmGrid::OnAddInCompanyClick,
                                  0, this);
  Layout();
}

void FrmGrid::OnAddInCompanyClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInCompanyClick"));
  try {

    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    ContraptionP contraption = contraptions->model()->New();
    FramesCollection::companies_frame->SetUpValues(contraption, contraptions);
    FramesCollection::companies_frame->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}
}
}
