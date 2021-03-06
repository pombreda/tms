#include "contraption_grid_table_base.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

ContraptionGridTableBase::ContraptionGridTableBase(ContraptionArrayP contraptions,
                                                   std::string name,
                                                   std::vector<Column> &cols) :
    contraption_factory_(boost::bind(&ContraptionGridTableBase::DefaultFactory, this)),
    contraptions_(contraptions), model_(), cols_(cols),
    printer_(), old_size_(0), timer_(new wxTimer()),
    contraption_dialog_(0), name_(name) {
  Init();
}

ContraptionGridTableBase::ContraptionGridTableBase(ContraptionArrayP contraptions,
                                                   std::vector<Column> &cols) :
    contraption_factory_(boost::bind(&ContraptionGridTableBase::DefaultFactory, this)),
    contraptions_(contraptions), model_(), cols_(cols),
    printer_(), old_size_(0), timer_(new wxTimer()),
    contraption_dialog_(0), name_("") {
  Init();
}

void ContraptionGridTableBase::Init() {
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
  InitContraptions();
}

void ContraptionGridTableBase::InitContraptions() {
  contraptions_->SetOnChange(boost::bind
			     (&ContraptionGridTableBase::OnChange, this));
}


wxString ContraptionGridTableBase::GetValue(int row, int col) {
  if (model_->GetField(
          cols_[static_cast<long unsigned>(col)].field_id)->IsReadable()) {
    wxASSERT(static_cast<size_t>(row) < contraptions_->size());
    return wxString::FromUTF8(
        printer_[col]->ToString(*(contraptions_->at(row)->
                                  GetFieldValue(cols_[col].field_id))).c_str());
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
  return wxString::FromUTF8(cols_[static_cast<long unsigned>(col)].name.c_str());
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

void ContraptionGridTableBase::StartTimer(int interval) {
  GetView()->Bind(wxEVT_TIMER, &ContraptionGridTableBase::OnTimer, this);
  timer_->SetOwner(GetView());
  timer_->Start(interval);
}

void ContraptionGridTableBase::StopTimer() {
  timer_->Stop();
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
  GetView()->Refresh();
}

void ContraptionGridTableBase::Refresh() {
  if (GetView() != NULL && GetView()->IsShownOnScreen()) {
    contraptions_->Refresh();
  }
}

void ContraptionGridTableBase::OnTimer(wxTimerEvent &WXUNUSED(event)) {
  Refresh();
}

}
}
}



