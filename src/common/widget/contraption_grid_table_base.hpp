#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_GRID_TABLE_BASE_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_GRID_TABLE_BASE_HPP_

// wxWidget
#include <wx/grid.h>
#include <wx/event.h>
#include <wx/timer.h>
// boost
#include <boost/signals.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
// std
#include <cstdio>
#include <vector>
// common
#include <contraption/model.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/field.hpp>
#include <contraption/field_type.hpp>
#include <widget/contraption_dialog.hpp>
#include "printer.hpp"
#include "column.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

class ContraptionGridTableBase : public wxGridTableBase {
 public:
  ContraptionGridTableBase(ContraptionArrayP contraptions,
                           std::string name,
                           std::vector<Column> &cols);
  ContraptionGridTableBase(ContraptionArrayP contraptions,
                           std::vector<Column> &cols);
  ~ContraptionGridTableBase() {}

  wxString GetValue(int row, int col);
  void SetValue(int row, int col, const wxString &value);
  int GetNumberCols();
  int GetNumberRows();
  wxString GetColLabelValue(int col);
  bool AppendRows(size_t numRows = 1);
  bool DeleteRows(size_t pos = 0, size_t numRows = 1);
  void RefreshViewColumns();
  void StartTimer(int interval);
  void StopTimer();           
  void set_contraption_dialog(ContraptionDialog *contraption_dialog) {
    contraption_dialog_ = contraption_dialog;
  }

  ContraptionDialog *contraption_dialog() {
    return contraption_dialog_;
  }
  
  const ContraptionArrayP contraptions() {
    return contraptions_;
  }

  const ModelP model() {
    return model_;
  }

  const std::string name() {
    return name_;
  }

  const std::vector<Column> cols() {
    return cols_;
  }

 private:
  ContraptionArrayP contraptions_;
  ModelP model_;
  std::vector<Column> cols_;
  std::vector<boost::shared_ptr<Printer> > printer_;
  int old_size_;
  wxTimer* timer_;
  ContraptionDialog *contraption_dialog_;
  std::string name_;
  void Init();
  void OnChange();
  void OnTimer(wxTimerEvent &event);

};

}
}
}
#endif // _TMS_COMMON_WIDGET__CONTRAPTION_GRID_TABLE_BASE_HPP_
