#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_

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

class ContraptionGrid : public wxGrid {
 public:
  typedef boost::function<void(ContraptionP contraption,
                               FieldID field_id)> OnClickFunction;

  ContraptionGrid(ContraptionArrayP contraptions,
                  std::vector<Column> &cols,
                  wxWindow *parent, wxWindowID id,
                  const wxPoint &pos=wxDefaultPosition,
                  const wxSize &size=wxDefaultSize,
                  long style=wxWANTS_CHARS,
                  const wxString &name=wxGridNameStr);
  virtual ~ContraptionGrid();
  void SetOnCellClick(OnClickFunction on_cell_click);
  void SetOnCellDClick(OnClickFunction on_cell_dclick);

 private:
  ContraptionGrid(const ContraptionGrid&);
  ContraptionGrid& operator=(const ContraptionGrid&);

  wxWindow *parent_;
  ContraptionArrayP contraptions_;
  std::vector<bool> contraprions_drawn_;
  ModelP model_;
  std::vector<Column> cols_;
  std::vector<boost::shared_ptr<Printer> > printer_;
  boost::signal<void(ContraptionP contraption,
                     FieldID field_id)> on_cell_click_;
  boost::signal<void(ContraptionP contraption,
                     FieldID field_id)> on_cell_dclick_;

 private:
  void LoadData();
  void BindListeners();
  void DrawContent(int min_row, int max_row);
  void OnUpdateView();
  void OnMove(wxMoveEvent &e);
  void OnPaint(wxPaintEvent &e);
  void OnSize(wxSizeEvent &e);
  void OnScrollWin(wxScrollWinEvent &e);
  void OnCellClick(wxGridEvent &e);
  void OnCellDClick(wxGridEvent &e);
  void OnChange();
};

}
}
}

#endif // _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
