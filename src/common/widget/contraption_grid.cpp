#include "contraption_grid.hpp"
#include <iostream>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGrid::ContraptionGrid(ContraptionArrayP contraptions,
                                 std::vector<Column> &cols,
                                 wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 long style, const wxString &name) :
    wxGrid(parent, id, pos, size, style, name),
    contraptions_(contraptions), cols_(cols),
    on_cell_click_(), on_cell_dclick_() {
  EnableDragColSize();
  EnableDragColMove();
  EnableDragRowSize();
  DisableCellEditControl();
  EnableEditing(false);
  ContraptionGridTableBase *base =
    new ContraptionGridTableBase(contraptions_, cols_);
  SetTable(base, true, wxGridSelectRows);
  for (size_t j = 0; j < cols.size(); j++) {
    SetColSize(j, cols_[j].width);
  }
  Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ContraptionGrid::OnCellClick, this);
  Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &ContraptionGrid::OnCellDClick, this);
  contraptions_->SetOnChange(boost::bind(&ContraptionGrid::OnChange, this));
}

ContraptionGrid::~ContraptionGrid() {
}

void ContraptionGrid::SetOnCellClick(OnClickFunction on_cell_click) {
  on_cell_click_.connect(on_cell_click);
}

void ContraptionGrid::SetOnCellDClick(OnClickFunction on_cell_dclick) {
  on_cell_dclick_.connect(on_cell_dclick);
}

void ContraptionGrid::OnCellClick(wxGridEvent &e) {
  size_t row = e.GetRow();
  size_t col = e.GetCol();
  SelectRow(row);
  ContraptionP &contraption = contraptions_->at(row);
  FieldID field_id = cols_[col].field_id;
  on_cell_click_(contraption, field_id);
}

void ContraptionGrid::OnCellDClick(wxGridEvent &e) {
  size_t row = e.GetRow();
  size_t col = e.GetCol();
  SelectRow(row);
  ContraptionP &contraption = contraptions_->at(row);
  FieldID field_id = cols_[col].field_id;
  on_cell_dclick_(contraption, field_id);
}

void ContraptionGrid::OnChange() {
  ForceRefresh();
}

}
}
}
