#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_

// wxWidget
#include <wx/grid.h>
#include <wx/event.h>
// boost
#include <boost/signals.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
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

class ContraptionGrid : public wxGrid {
 public:
  ContraptionGrid(ContraptionArrayP &contraptions,
                  std::vector<Column> &cols,
                  wxWindow *parent, wxWindowID id,
                  const wxPoint &pos=wxDefaultPosition,
                  const wxSize &size=wxDefaultSize,
                  long style=wxWANTS_CHARS,
                  const wxString &name=wxGridNameStr);
  virtual ~ContraptionGrid();
  void SetOnCellClick(boost::function<void(ContraptionP&, FieldID)> on_cell_click);

 private:
  ContraptionArrayP contraptions_;
  bool* contraprions_drawn_;
  ModelP model_;
  std::vector<Column> cols_;
  Printer** printer_;
  boost::function<void(ContraptionP& contraption, FieldID field_id)> on_cell_click_;

 private:
  void DrawContent(int min_row, int max_row);
  void OnUpdateView(wxPaintEvent &e);
  void OnCellClick(wxGridEvent &e);
  void OnChange();
};

}
}
}

#endif // _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
