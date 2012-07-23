#ifndef GRIDFRAME_H
#define GRIDFRAME_H

//(*Headers(GridFrame)
#include <wx/frame.h>
#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/sizer.h>
//*)

#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/contraption_array.hpp>
#include <widget/contraption_grid.hpp>

// soci
#include <soci/sqlite3/soci-sqlite3.h>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace tms::common::widget;
using namespace tms::common::contraption;

class GridFrame: public wxFrame {
 public:
	virtual ~GridFrame();
	void Init();

 private:
  GridFrame(const GridFrame&);
  GridFrame& operator=(const GridFrame&);

	ContraptionGrid* grid_;

  DECLARE_EVENT_TABLE()
};

#endif