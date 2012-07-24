#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_GRID_TABLE_BASE_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_GRID_TABLE_BASE_HPP_

// wxWidget
#include <wx/grid.h>
#include <wx/event.h>
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
#include "printer.hpp"
#include "column.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

class ContraptionGridTableBase : public wxGridTableBase {
 public:
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

 private:
  ContraptionArrayP contraptions_;
  ModelP model_;
  std::vector<Column> cols_;
  std::vector<boost::shared_ptr<Printer> > printer_;

};

}
}
}
#endif // _TMS_COMMON_WIDGET__CONTRAPTION_GRID_TABLE_BASE_HPP_
