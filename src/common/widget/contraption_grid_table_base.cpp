#include "contraption_grid_table_base.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGridTableBase::ContraptionGridTableBase(ContraptionArrayP contraptions,
                                                   std::vector<Column> &cols) :
    contraptions_(contraptions), model_(), cols_(cols), printer_() {
  model_ = contraptions_->model();
  for (size_t j = 0; j < cols_.size(); j++) {
    FieldID i = cols_[j].field_id;
    if (dynamic_cast<const FieldT<int>*>(model_->GetField(i)) != 0) {
      printer_.push_back(boost::shared_ptr<Printer>(new PrinterT<int>()));
    } else if (dynamic_cast<const FieldT<std::string>*>(model_->GetField(i)) != 0) {
      printer_.push_back(boost::shared_ptr<Printer>(new PrinterT<std::string>()));
    }
  }
}

wxString ContraptionGridTableBase::GetValue(int row, int col) {
  std::cerr << "Getting value at " << row << ", " << col << std::endl;
  if (model_->GetField(cols_[col].field_id)->IsReadable()) {
    return printer_[col]->ToString(*(contraptions_->at(row)->
                                     GetFieldValue(cols_[col].field_id)));
  } else {
    return _T("###");
  }
}

void ContraptionGridTableBase::SetValue(int row, int col, const wxString &value) {
  std::cerr << "Setting value " << value.ToStdString() << " at " <<
    row << ", " << col << std::endl;
}

int ContraptionGridTableBase::GetNumberCols() {
  return cols_.size();
}

int ContraptionGridTableBase::GetNumberRows() {
  return contraptions_->size();
}

wxString ContraptionGridTableBase::GetColLabelValue(int col) {
  return cols_[col].name;
}

}
}
}



