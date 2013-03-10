#include "contraption_grid.hpp"
// std
#include <sstream>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGrid::ContraptionGrid(wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 long style, const wxString &name) :
    wxGrid(parent, id, pos, size, style, name),
    layout_(0),
    add_button_(0),
    table_choice_(0),
    bases_(), on_cell_click_(), on_cell_dclick_(), selmode_(wxGridSelectRows), interval_(2500), id_(-1) {
  EnableDragColSize();
  EnableDragColMove();
  EnableDragRowSize();
  DisableCellEditControl();
  EnableEditing(false);
  SetDefaultRenderer(new wxGridCellAutoWrapStringRenderer());
  dlg_check_column_ = new DlgCheckColumn(this);
  Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ContraptionGrid::OnCellClick, this);
  Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &ContraptionGrid::OnCellDClick, this);
  Bind(wxEVT_GRID_LABEL_RIGHT_CLICK, &ContraptionGrid::OnLabelRightClick, this);
}

ContraptionGrid::~ContraptionGrid() {
  if (GetTable() != NULL) {
    dynamic_cast<ContraptionGridTableBase*>(GetTable())->StopTimer();
  }  
}

bool ContraptionGrid::ChooseTable(int id) {
  SaveLayout();
  ReleaseTable();
  ResetColPos();
  if (static_cast<size_t>(id) < bases_.size()) {
    id_ = id;
    bool res = wxGrid::SetTable(bases_[id_], false, selmode_);
    bases_[id_]->RefreshViewColumns();
    bases_[id_]->Refresh();
    bases_[id_]->StartTimer(interval_);
    UpdateChoice();
    LoadLayout();
    return res;
  }
  return false;
}

void ContraptionGrid::SaveLayout() {
  if (static_cast<size_t>(id_) < bases_.size()) {
    TableLayout table_layout(GetNumberCols());
    for (int col = 0; col < GetNumberCols(); ++col) {
      ColumnLayout column_layout;
      if (GetColSize(col) != 0) {
        column_layout.width = GetColSize(col);
        column_layout.enabled = true;
      } else {
        column_layout.enabled = false;
      }
      column_layout.pos = GetColPos(col);
      table_layout[col] = column_layout;
    }
    layout_[id_] = table_layout;
  }
}

void ContraptionGrid::LoadLayout() {
  if (static_cast<size_t>(id_) < bases_.size()) {
    TableLayout table_layout = layout_[id_];  
    if (table_layout.size() != static_cast<size_t>(GetNumberCols())) {
      return;
    }
    for (int col = 0; col < GetNumberCols(); ++col) {
      ColumnLayout column_layout = table_layout[col];
      if (column_layout.enabled) {
        SetColSize(col, column_layout.width);
      } else {
        SetColSize(col, 0);
      }
      SetColPos(col, column_layout.pos);
    }
  }
}

void ContraptionGrid::ReleaseTable() {
  if (id_ >= 0) {
    dynamic_cast<ContraptionGridTableBase*>(bases_[id_])->StopTimer();
    id_ = -1;
  }
}

void ContraptionGrid::AddTable(ContraptionGridTableBase *table) {
  bases_.push_back(table);
  layout_.push_back(TableLayout());
  UpdateChoice();
}

bool ContraptionGrid::SetTable(ContraptionGridTableBase *table) {
  ReleaseTable();
  bases_.clear();
  layout_.clear();
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
      ContraptionP contraption = bases_[id_]->contraption_factory()();
      assert(bases_[id_]->contraptions()->model() == contraption->model());
      dialog->SetUpValues(contraption,
                          bases_[id_]->contraptions());
      dialog->ShowModal();   
    }
  }
}

void ContraptionGrid::ShowCol(int col, bool show) {
  if (show) {
    if (layout_[id_].size() > static_cast<size_t>(col)) {
      SetColSize(col, layout_[id_][col].width);
    }
  } else {
    SaveLayout();
    SetColSize(col, 0);
  }
}

void ContraptionGrid::OnLabelRightClick(wxGridEvent &event) {
  dlg_check_column_->SetUpValues(this);
  wxPoint position = event.GetPosition();
  wxWindow *win= this;
  while (win) {
    position += win->GetPosition();
    win = win->GetParent();
  }
  dlg_check_column_->Move(position);
  dlg_check_column_->Popup();
}

void ContraptionGrid::OnCellDClick(wxGridEvent &event) {
  if (id_ >= 0) {
    int row = event.GetRow();
    int col = event.GetCol();
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

std::string ContraptionGrid::SerializeLayout() {
  std::ostringstream sout;
  sout << layout_.size() << " ";
  for (size_t table = 0; table < layout_.size(); ++table) {
    TableLayout table_layout = layout_[table];
    sout << table_layout.size() << " ";
    for (size_t col = 0; col < table_layout.size(); ++col) {
      ColumnLayout column_layout = table_layout[col];
      sout << column_layout.width << " "
           << column_layout.pos << " "
           << column_layout.enabled << " ";
    }
  }
  return sout.str();
}

void ContraptionGrid::DeserializeLayout(std::string data) {
  std::istringstream sin(data);
  layout_.clear();
  size_t layout_size;
  sin >> layout_size;
  for (size_t table = 0; table < layout_size; ++table) {
    TableLayout table_layout(0);
    size_t table_layout_size;
    sin >> table_layout_size;
    for (size_t col = 0; col < table_layout_size; ++col) {
      ColumnLayout column_layout;
      sin >> column_layout.width
          >> column_layout.pos
          >> column_layout.enabled;
      table_layout.push_back(column_layout);
    }
    layout_.push_back(table_layout);
  }
}

}
}
}
