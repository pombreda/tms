#include "contraption_grid.hpp"

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
  contraptions_(contraptions), model_(model), cols_(cols)  {
  int cols_number = cols_.size();
  int rows_number = contraptions_->size();

  CreateGrid(0, 0);
  AppendRows(rows_number);
  AppendCols(cols_number);

  Printer* printer[cols_number];
  for (int j = 0; j < cols_number; j++) {
    int i = cols_[j].field_id;
    if (dynamic_cast<const FieldT<int>*>(model_->GetField(i)) != 0) {
      printer[j] = new PrinterT<int>();
    } else if (dynamic_cast<const FieldT<std::string>*>(model_->GetField(i)) != 0) {
      printer[j] = new PrinterT<std::string>();
    }
    SetColLabelValue(j, _T(cols_[j].name));
    SetColSize(j, cols_[j].width);
  }

  for (int i = 0; i < rows_number; i++) {
    for (int j = 0; j < cols_number; j++) {
      if (model_->GetField(cols_[j].field_id)->IsReadable()) {
        SetCellValue(i, j, _T(printer[j]->ToString(*(contraptions_->at(i)->
                                                     GetFieldValue(cols_[j].field_id)))));
      } else {
        SetCellValue(i, j, _T("Unreadable value!"));
      }
    }
  }
}

ContraptionGrid::~ContraptionGrid() {
  //dtor
}

}
}
}
