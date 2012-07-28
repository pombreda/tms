#include "contraption_grid_table_base.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGridTableBase::ContraptionGridTableBase(ContraptionArrayP contraptions,
                                                   std::vector<Column> &cols) :
    contraptions_(contraptions), model_(), cols_(cols),
    printer_(), old_size_(0) {
  model_ = contraptions_->model();
  old_size_ = contraptions_->size();
  for (size_t j = 0; j < cols_.size(); j++) {
    FieldID i = cols_[j].field_id;
    if (dynamic_cast<const FieldT<int>*>(model_->GetField(i)) != 0) {
      printer_.push_back(boost::shared_ptr<Printer>(new PrinterT<int>()));
    } else if (dynamic_cast<const FieldT<std::string>*>(model_->GetField(i)) != 0) {
      printer_.push_back(boost::shared_ptr<Printer>(new PrinterT<std::string>()));
    }
  }
  contraptions_->SetOnChange(boost::bind
                             (&ContraptionGridTableBase::OnChange, this));
}

wxString ContraptionGridTableBase::GetValue(int row, int col) {
  if (model_->GetField(cols_[static_cast<long unsigned>(col)].field_id)->IsReadable()) {
    return printer_[col]->ToString(*(contraptions_->at(row)->
                                     GetFieldValue(cols_[col].field_id)));
  } else {
    return _T("###");
  }
}

void ContraptionGridTableBase::SetValue(int /*row*/, int /*col*/,
                                        const wxString &/*value*/) {
  // ignore
}

int ContraptionGridTableBase::GetNumberCols() {
  return static_cast<int>(cols_.size());
}

int ContraptionGridTableBase::GetNumberRows() {
  return static_cast<int>(contraptions_->size());
}

wxString ContraptionGridTableBase::GetColLabelValue(int col) {
  return cols_[static_cast<long unsigned>(col)].name;
}

bool ContraptionGridTableBase::AppendRows(size_t numRows) {
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED,
                         static_cast<int>(numRows));
  GetView()->ProcessTableMessage(msg);
  return true;
}

bool ContraptionGridTableBase::DeleteRows(size_t pos, size_t numRows) {
  wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED,
                         static_cast<int>(pos), static_cast<int>(numRows));
  GetView()->ProcessTableMessage(msg);
  return true;
}

void ContraptionGridTableBase::RefreshViewColumns() {
  for (size_t j = 0; j < cols_.size(); j++) {
    GetView()->SetColSize(j, cols_[static_cast<int>(j)].width);
  }
}

void ContraptionGridTableBase::OnChange() {
  int delta = static_cast<int>(old_size_) -
    static_cast<int>(contraptions_->size());
  if (delta < 0) {
    GetView()->AppendRows(-delta);
  } else if (delta > 0) {
    GetView()->DeleteRows(static_cast<int>(contraptions_->size()), delta);
  }
  old_size_ = contraptions_->size();
}

}
}
}



