#include "contraption_grid.hpp"
#include <iostream>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGrid::ContraptionGrid(ContraptionArrayP &contraptions, ModelP &model,
                                 std::vector<Column> &cols,
                                 wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 long style, const wxString &name) :
  wxGrid(parent, id, pos, size, style, name),
  contraptions_(contraptions), model_(model), cols_(cols) {

  EnableDragColSize();
  EnableDragColMove();
  EnableDragRowSize();
  DisableCellEditControl();
  EnableEditing(false);

  int cols_number = cols_.size();
  int rows_number = contraptions_->size();

  CreateGrid(rows_number, cols_number, wxGridSelectRows);

  printer_ = new Printer*[cols_number];
  for (int j = 0; j < cols_number; j++) {
    int i = cols_[j].field_id;
    if (dynamic_cast<const FieldT<int>*>(model_->GetField(i)) != 0) {
      printer_[j] = new PrinterT<int>();
    } else if (dynamic_cast<const FieldT<std::string>*>(model_->GetField(i)) != 0) {
      printer_[j] = new PrinterT<std::string>();
    }
    SetColLabelValue(j, cols_[j].name);
    SetColSize(j, cols_[j].width);
  }

  Bind(wxEVT_PAINT, &ContraptionGrid::OnUpdateView, this);
}

ContraptionGrid::~ContraptionGrid() {
  for (int i = 0; i < cols_.size(); i++) {
    delete printer_[i];
  }
  delete printer_;
}

void ContraptionGrid::DrawContent(int min_row) {
  int cols_number = cols_.size();
  int rows_number = contraptions_->size();
  for (int i = min_row; i < rows_number; i++) {
    if (IsVisible(i, 0, false)) {
      for (int j = 0; j < cols_number; j++) {
        if (model_->GetField(cols_[j].field_id)->IsReadable()) {
          SetCellValue(i, j, printer_[j]->ToString(*(contraptions_->at(i)->
                                                       GetFieldValue(cols_[j].field_id))));
        } else {
          SetCellValue(i, j, _T("Unreadable value!"));
        }
      }
    }
  }
}

void ContraptionGrid::OnUpdateView(wxPaintEvent &e) {
  std::cerr << "onpaint" << std::endl;
  DrawContent(0);
}

}
}
}
