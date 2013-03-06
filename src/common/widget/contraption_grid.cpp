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
    base_(), on_cell_click_(), on_cell_dclick_() {
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
}

bool ContraptionGrid::SetTable(ContraptionGridTableBase *table,
                               wxGridSelectionModes selmode,
                               int interval) {
  if (GetTable() != NULL) {
    dynamic_cast<ContraptionGridTableBase*>(GetTable())->StopTimer();
  }
  bool res = wxGrid::SetTable(table, false, selmode);
  base_ = table;
  base_->RefreshViewColumns();
  base_->StartTimer(interval);
  return res;
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
  if (base_ != NULL) {
    int row = e.GetRow();
    int col = e.GetCol();
    SelectRow(row);
    ContraptionP contraption = base_->contraptions()->at(static_cast<size_t>(row));
    FieldID field_id = static_cast<FieldID>(base_->cols()[col].field_id);
    on_cell_click_(contraption, field_id);
  }
}

void ContraptionGrid::OnCellDClick(wxGridEvent &e) {
  if (base_ != NULL) {
    int row = e.GetRow();
    int col = e.GetCol();
    SelectRow(row);
    ContraptionP contraption = base_->contraptions()->at(static_cast<size_t>(row));
    FieldID field_id = static_cast<FieldID>(base_->cols()[col].field_id);
    on_cell_dclick_(contraption, field_id);
  }
}

}
}
}
