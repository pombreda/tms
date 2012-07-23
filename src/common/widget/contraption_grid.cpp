#include "contraption_grid.hpp"
#include <iostream>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGrid::ContraptionGrid(ContraptionArrayP &contraptions,
                                 std::vector<Column> &cols,
                                 wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 long style, const wxString &name) :
    wxGrid(parent, id, pos, size, style, name),
    contraptions_(contraptions), model_(), cols_(cols),
    on_cell_click_(), contraprions_drawn_(), printer_() {
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
  contraprions_drawn_ = new bool[rows_number];
  for (size_t i = 0; i < rows_number; ++i) {
    contraprions_drawn_[i] = false;
  }

  printer_ = new Printer*[cols_number];
  for (size_t j = 0; j < cols_number; j++) {
    FieldID i = cols_[j].field_id;
    if (dynamic_cast<const FieldT<int>*>(model_->GetField(i)) != 0) {
      printer_[j] = new PrinterT<int>();
    } else if (dynamic_cast<const FieldT<std::string>*>(model_->GetField(i)) != 0) {
      printer_[j] = new PrinterT<std::string>();
    }
    SetColLabelValue(j, cols_[j].name);
    SetColSize(j, cols_[j].width);
  }
}

void ContraptionGrid::BindListeners() {
  Bind(wxEVT_PAINT, &ContraptionGrid::OnUpdateView, this);
  Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ContraptionGrid::OnCellClick, this);
  contraptions_->SetOnChange(boost::bind(&ContraptionGrid::OnChange, this));
}

ContraptionGrid::~ContraptionGrid() {
  if (printer_) {
    for (size_t i = 0; i < cols_.size(); i++) {
      delete printer_[i];
    }
    delete printer_;
  }
}

void ContraptionGrid::SetOnCellClick(boost::function<
                                     void(ContraptionP&, FieldID)> on_cell_click) {
  on_cell_click_ = on_cell_click;
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

void ContraptionGrid::OnUpdateView(wxPaintEvent &WXUNUSED(e)) {
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

void ContraptionGrid::OnCellClick(wxGridEvent &e) {
  size_t row = e.GetRow();
  size_t col = e.GetCol();
  ContraptionP &contraption = contraptions_->at(row);
  FieldID field_id = cols_[col].field_id;
  if (on_cell_click_ != 0)
    on_cell_click_(contraption, field_id);
}

void ContraptionGrid::OnChange() {
  size_t rows_number = contraptions_->size();
  contraprions_drawn_ = new bool[rows_number];
  for (size_t i = 0; i < rows_number; ++i) {
    contraprions_drawn_[i] = false;
  }
  int delta = rows_number - GetNumberRows();
  if (delta > 0) {
    AppendRows(delta);
  } else if (delta < 0) {
    DeleteRows(rows_number, delta);
  }
  wxPaintEvent e;
  OnUpdateView(e);
}

}
}
}
