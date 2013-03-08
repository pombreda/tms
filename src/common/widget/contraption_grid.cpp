#include "contraption_grid.hpp"
#include <iostream>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGrid::ContraptionGrid(wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 long style, const wxString &name) :
    wxGrid(parent, id, pos, size, style, name),
    add_button_(0),
    table_choice_(0),
    bases_(), on_cell_click_(), on_cell_dclick_(), selmode_(wxGridSelectRows), interval_(2500), id_(-1) {
  EnableDragColSize();
  EnableDragColMove();
  EnableDragRowSize();
  DisableCellEditControl();
  EnableEditing(false);
  SetDefaultRenderer(new wxGridCellAutoWrapStringRenderer());
  Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ContraptionGrid::OnCellClick, this);
  Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &ContraptionGrid::OnCellDClick, this);
}

ContraptionGrid::~ContraptionGrid() {
  if (GetTable() != NULL) {
    dynamic_cast<ContraptionGridTableBase*>(GetTable())->StopTimer();
  }  
}

bool ContraptionGrid::ChooseTable(int id) {
  ReleaseTable();
  ResetColPos();
  id_ = id;
  if (static_cast<size_t>(id_) < bases_.size()) {
    bool res = wxGrid::SetTable(bases_[id_], false, selmode_);
    bases_[id_]->RefreshViewColumns();
    bases_[id_]->StartTimer(interval_);
    UpdateChoice();
    return res;
  }
  return false;
}

void ContraptionGrid::ReleaseTable() {
  if (id_ >= 0) {
    dynamic_cast<ContraptionGridTableBase*>(bases_[id_])->StopTimer();
    id_ = -1;
  }
}

void ContraptionGrid::AddTable(ContraptionGridTableBase *table) {
  bases_.push_back(table);
  UpdateChoice();
}

bool ContraptionGrid::SetTable(ContraptionGridTableBase *table) {
  ReleaseTable();
  bases_.clear();
  AddTable(table);
  return ChooseTable(0);
}

void ContraptionGrid::SetOnCellClick(OnClickFunction on_cell_click) {
  on_cell_click_.disconnect_all_slots();
  on_cell_click_.connect(on_cell_click);
}

void ContraptionGrid::SetOnCellDClick(OnClickFunction on_cell_dclick) {
  on_cell_dclick_.disconnect_all_slots();
  on_cell_dclick_.connect(on_cell_dclick);
}

void ContraptionGrid::OnCellClick(wxGridEvent &e) {
  if (id_ >= 0) {
    int row = e.GetRow();
    int col = e.GetCol();
    SelectRow(row);
    ContraptionP contraption = bases_[id_]->
                               contraptions()->at(static_cast<size_t>(row));
    FieldID field_id = static_cast<FieldID>(
        bases_[id_]->cols()[col].field_id);
    on_cell_click_(contraption, field_id);
  }
}

void ContraptionGrid::AddContraption() {
  if (id_ >= 0) {
    ContraptionDialog *dialog = bases_[id_]->contraption_dialog();
    if (dialog) {
      dialog->SetUpValues(bases_[id_]->contraptions()->model()->New(),
                          bases_[id_]->contraptions());
      dialog->ShowModal();   
    }
  }
}

void ContraptionGrid::OnCellDClick(wxGridEvent &e) {
  if (id_ >= 0) {
    int row = e.GetRow();
    int col = e.GetCol();
    SelectRow(row);
    ContraptionP contraption = bases_[id_]->
                               contraptions()->at(static_cast<size_t>(row));
    FieldID field_id = static_cast<FieldID>(
        bases_[id_]->cols()[col].field_id);
    ContraptionDialog *dialog = bases_[id_]->contraption_dialog();
    if (dialog && on_cell_dclick_.empty()) {
      dialog->SetUpValues(contraption, bases_[id_]->contraptions());
      dialog->ShowModal();
    } else {
      on_cell_dclick_(contraption, field_id);
    }
  }
}

void ContraptionGrid::set_table_choice(wxChoice *choice) {
  choice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, 
               boost::bind(&ContraptionGrid::OnChoiceChange, this, _1));
  table_choice_ = choice;
}

void ContraptionGrid::OnChoiceChange(wxCommandEvent &event) {
  ChooseTable(event.GetSelection());
}

void ContraptionGrid::UpdateChoice() {
  if (table_choice_) {
    table_choice_->Clear();
    for (size_t id = 0; id < bases_.size(); ++id) {
      table_choice_->Append(wxString::FromUTF8(bases_[id]->name().c_str()));
    }
    table_choice_->SetSelection(id_);
  }
}

}
}
}
