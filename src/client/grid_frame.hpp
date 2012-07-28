#ifndef _TMS_CLIENT__GRID_FRAME_HPP_
#define _TMS_CLIENT__GRID_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/sizer.h>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
// std
#include <string>
#include <vector>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/contraption_array.hpp>
#include <widget/contraption_grid.hpp>
#include "test_frame.hpp"

namespace tms {
namespace client {

class GridFrame : public wxFrame {
 public:
  GridFrame() :
      wxFrame(), grid_(), contraptions_(), test_frame() {}
  virtual ~GridFrame();
  void Init(tms::common::contraption::ModelP model, 
            const std::vector<Column> &cols);

 private:
  GridFrame(const GridFrame&);
  GridFrame& operator=(const GridFrame&);

  tms::common::widget::ContraptionGrid* grid_;
  tms::common::contraption::ContraptionArrayP contraptions_;
  TestFrame* test_frame;

  void OnCellClick(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::FieldID field_id);
  void OnCellDClick(tms::common::contraption::ContraptionP contraption,
                    tms::common::contraption::FieldID field_id);
  void OnAddClick(wxCommandEvent& WXUNUSED(event));
  void OnExitClick(wxCommandEvent& WXUNUSED(event));

  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__GRID_FRAME_HPP_
