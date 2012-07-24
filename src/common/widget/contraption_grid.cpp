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
    parent_(parent),
    contraptions_(contraptions), model_(), cols_(cols),
    on_cell_click_(), on_cell_dclick_(),
    contraprions_drawn_(), printer_() {
  EnableDragColSize();
  EnableDragColMove();
  EnableDragRowSize();
  DisableCellEditControl();
  EnableEditing(false);
  LoadData();
  BindListeners();
}

void ContraptionGrid::LoadData() {
  size_t cols_number = cols_.size();
  size_t rows_number = contraptions_->size();
  model_ = contraptions_->model();

  CreateGrid(rows_number, cols_number, wxGridSelectRows);
  for (size_t i = 0; i < rows_number; ++i) {
    contraprions_drawn_.push_back(false);
  }

  for (size_t j = 0; j < cols_number; j++) {
    FieldID i = cols_[j].field_id;
    if (dynamic_cast<const FieldT<int>*>(model_->GetField(i)) != 0) {
      printer_.push_back(boost::shared_ptr<Printer>(new PrinterT<int>()));
    } else if (dynamic_cast<const FieldT<std::string>*>(model_->GetField(i)) != 0) {
      printer_.push_back(boost::shared_ptr<Printer>(new PrinterT<std::string>()));
    }
    SetColLabelValue(j, cols_[j].name);
    SetColSize(j, cols_[j].width);
  }
}

void ContraptionGrid::BindListeners() {
  parent_->Bind(wxEVT_MOVE, &ContraptionGrid::OnMove, this);
  parent_->Bind(wxEVT_MOVING, &ContraptionGrid::OnMove, this);
  Bind(wxEVT_PAINT, &ContraptionGrid::OnPaint, this);
  Bind(wxEVT_SIZE, &ContraptionGrid::OnSize, this);
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

void ContraptionGrid::DrawContent(int min_row, int max_row) {
  size_t cols_number = cols_.size();
  for (size_t i = min_row; i < max_row; i++) {
    if (!contraprions_drawn_[i]) {
      for (FieldID j = 0; j < cols_number; j++) {
        if (model_->GetField(cols_[j].field_id)->IsReadable()) {
          SetCellValue(i, j, printer_[j]->ToString(*(contraptions_->at(i)->
                                                       GetFieldValue(cols_[j].field_id))));
        } else {
          SetCellValue(i, j, _T("###"));
        }
      }
      contraprions_drawn_[i] = true;
    }
  }
}

void ContraptionGrid::OnUpdateView() {
  ForceRefresh();
  int x = 0;
  int y = 0;
  CalcUnscrolledPosition(0, 0, &x, &y);
  int min_row = YToRow(y);
  int w = 0;
  int h = 0;
  GetSize(&w, &h);
  CalcUnscrolledPosition(0, h, &x, &y);
  int max_row = YToRow(y);
  if (max_row == -1) max_row = contraptions_->size();
  DrawContent(min_row, max_row);
}

void ContraptionGrid::OnMove(wxMoveEvent &WXUNUSED(e)) {
  OnUpdateView();
}

void ContraptionGrid::OnPaint(wxPaintEvent &WXUNUSED(e)) {
  OnUpdateView();
}

void ContraptionGrid::OnSize(wxSizeEvent &WXUNUSED(e)) {
  OnUpdateView();
}

void ContraptionGrid::OnScrollWin(wxScrollWinEvent &WXUNUSED(e)) {
  OnUpdateView();
}

void ContraptionGrid::OnCellClick(wxGridEvent &e) {
  size_t row = e.GetRow();
  size_t col = e.GetCol();
  ContraptionP &contraption = contraptions_->at(row);
  FieldID field_id = cols_[col].field_id;
  on_cell_click_(contraption, field_id);
}

void ContraptionGrid::OnCellDClick(wxGridEvent &e) {
  size_t row = e.GetRow();
  size_t col = e.GetCol();
  ContraptionP &contraption = contraptions_->at(row);
  FieldID field_id = cols_[col].field_id;
  on_cell_dclick_(contraption, field_id);
}

void ContraptionGrid::OnChange() {
  size_t rows_number = contraptions_->size();
  contraprions_drawn_.clear();
  for (size_t i = 0; i < rows_number; ++i) {
    contraprions_drawn_.push_back(false);
  }
  int delta = rows_number - GetNumberRows();
  if (delta > 0) {
    AppendRows(delta);
  } else if (delta < 0) {
    DeleteRows(rows_number, delta);
  }
  OnUpdateView();
}

}
}
}
