#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_

#include <wx/grid.h>
#include <contraption/model.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/field.hpp>
#include <contraption/field_type.hpp>
#include <cstdio>
#include "printer.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

class ContraptionGrid : public wxGrid {
 public:
  ContraptionGrid(ContraptionArrayP &contraptions, ModelP &model,
                  wxWindow *parent, wxWindowID id,
                  const wxPoint &pos=wxDefaultPosition,
                  const wxSize &size=wxDefaultSize,
                  long style=wxWANTS_CHARS,
                  const wxString &name=wxGridNameStr);
  virtual ~ContraptionGrid();
 private:
  ContraptionArrayP contraptions_;
  ModelP model_;
};

}
}
}

#endif // _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
