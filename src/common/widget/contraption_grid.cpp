#include "contraption_grid.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGrid::ContraptionGrid(ContraptionArrayP &contraptions, ModelP &model,
                                 wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 long style, const wxString &name) :
  wxGrid(parent, id, pos, size, style, name) {
  int k = model->GetFieldNumber();
  int n = contraptions->size();

  CreateGrid(0, 0);
  AppendRows(n);
  AppendCols(k);

  Printer* printer[k];
  for (int i = 0; i < k; i++) {
    if (dynamic_cast<const FieldT<int>*>(model->GetField(i)) != 0) {
      printer[i] = new PrinterT<int>();
    } else if (dynamic_cast<const FieldT<std::string>*>(model->GetField(i)) != 0) {
      printer[i] = new PrinterT<std::string>();
    }
    std::string name = model->GetField(i)->name();
    SetColLabelValue(i, _T(name));
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < k; j++) {
      if (model->GetField(j)->IsReadable()) {
        SetCellValue(i, j, _T(printer[j]->ToString(*(contraptions->at(i)->GetFieldValue(j)))));
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
