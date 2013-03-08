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

void FrmGrid::OnIncomingsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnIncomingsCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_books_->GetTable())->
    contraptions();
  FramesCollection::incomings_frame->SetUpValues(contraption, contraptions);
  FramesCollection::incomings_frame->Show(true);  
}

void FrmGrid::OnAddInIncomingClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInIncomingClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_books_->GetTable())->
      contraptions();
    ContraptionP contraption = contraptions->model()->New();
    FramesCollection::incomings_frame->SetUpValues(contraption, contraptions);
    FramesCollection::incomings_frame->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}

void FrmGrid::InitIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Incomings table"));
  try {
    
    std::vector<Column> cols;
    ModelP model = Incoming::GetModel();
    cols.push_back(Column(model->GetFieldID("ID"), "ID", 150));
    cols.push_back(Column(model->GetFieldID("company_name"), "Компания", 100));
    cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 150));
    cols.push_back(Column(model->GetFieldID("contact_email"), "E-mail", 150));
    cols.push_back(Column(model->GetFieldID("contact_phone"), "Телефон", 150));
    cols.push_back(Column(model->GetFieldID("manager_name"), "Адресат", 100));
    cols.push_back(Column(model->GetFieldID("type_in"), "Тип приема", 100));
    cols.push_back(Column(model->GetFieldID("time_in"), "Время приема", 100));
    cols.push_back(Column(model->GetFieldID("time_out"), "Время передачи", 100));
    table_incomings_ =
      new ContraptionGridTableBase(model->All(), cols);
    
  } catch (GUIException &e) {
    Report(e);
  }
}


void FrmGrid::ActivateIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating incomings table"));
  
  grid_books_->ResetColPos();
  grid_books_->SetTable(table_incomings_);
  grid_books_->SetOnCellDClick(boost::bind(&FrmGrid::OnIncomingsCellDClick,
					      this, _1, _2));  
  button_add_in_book_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
  button_add_in_book_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                               (wxObjectEventFunction)&FrmGrid::OnAddInIncomingClick,
                               0, this);
  int col_number = grid_books_->GetNumberCols();
  vector<int> col_by_pos(col_number, -1);
  for (int col = 0; col < col_number; ++col) {
    std::string col_name = grid_books_->GetColLabelValue(col).utf8_str().data();
    ColumnLayout column_layout = Options::incoming_column_layout(col_name);
    if (column_layout.width != -1) {
      grid_books_->SetColSize(col, column_layout.width);
    }
    if (!column_layout.enabled) {
      grid_books_->SetColSize(col, 0);
    }
    if (column_layout.pos != -1) {
      col_by_pos[column_layout.pos] = col;
    }
  }
  grid_books_->SetColSize(0, 30);
  for (int col_pos = 0; col_pos < col_number; ++col_pos) {
    int col = col_by_pos[col_pos];
    if (col != - 1) {
      grid_books_->SetColPos(col, col_pos);
    }
  }
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Incomings table activated"));
}

void FrmGrid::DeactivateIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Deactivating incomings table"));

  int col_number = grid_books_->GetNumberCols();
  for (int col = 0; col < col_number; ++col) {
    std::string col_name = grid_books_->GetColLabelValue(col).utf8_str().data();
    ColumnLayout column_layout;
    if (grid_books_->GetColSize(col) != 0) {
      column_layout.width = grid_books_->GetColSize(col);
      column_layout.enabled = true;
    } else {
      column_layout.enabled = false;
    }
    column_layout.pos = grid_books_->GetColPos(col);
    column_layout.enabled = true;
    Options::set_incoming_column_layout(col_name, column_layout);
  }
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Incomings table deactivated"));

}


}
}
