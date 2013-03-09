#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_

// wxWidget
#include <wx/grid.h>
#include <wx/event.h>
#include <wx/button.h>
#include <wx/choice.h>
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
#include <widget/dlg_check_column.hpp>
#include "printer.hpp"
#include "column.hpp"
#include "contraption_grid_table_base.hpp"

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

class ContraptionGrid : public wxGrid {
 public:
  struct ColumnLayout {
    int width;
    int pos;
    bool enabled;
    ColumnLayout() :
        width(-1),
        pos(-1),
        enabled(true) {}
  };
  typedef std::vector<ColumnLayout> TableLayout;

  typedef boost::function<void(ContraptionP contraption,
                               FieldID field_id)> OnClickFunction;

  ContraptionGrid(wxWindow *parent, wxWindowID id,
                  const wxPoint &pos=wxDefaultPosition,
                  const wxSize &size=wxDefaultSize,
                  long style=wxWANTS_CHARS,
                  const wxString &name=wxGridNameStr);
  virtual ~ContraptionGrid();
  void SetOnCellClick(OnClickFunction on_cell_click);
  void SetOnCellDClick(OnClickFunction on_cell_dclick);
  bool SetTable(ContraptionGridTableBase *table);
  void AddTable(ContraptionGridTableBase *table);
  bool ChooseTable(int id);

  void AddContraption();
  void set_add_button(wxButton *add_button) {
    add_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                     boost::bind(&ContraptionGrid::AddContraption,
                                 this));
  }
  
  void set_table_choice(wxChoice *table_choice);
  std::string SerializeLayout();
  void DeserializeLayout(std::string data);
  void ShowCol(int col, bool show);
 private:
  std::vector<TableLayout> layout_;
  DlgCheckColumn *dlg_check_column_;
  ContraptionGrid(const ContraptionGrid&);
  ContraptionGrid& operator=(const ContraptionGrid&);
  void OnLabelRightClick(wxGridEvent &event);
  void SaveLayout();
  void LoadLayout();
  void ReleaseTable();
  void UpdateChoice();
  wxButton *add_button_;
  wxChoice *table_choice_;
  std::vector<ContraptionGridTableBase *>bases_;
  boost::signal<void(ContraptionP contraption,
                     FieldID field_id)> on_cell_click_;
  boost::signal<void(ContraptionP contraption,
                     FieldID field_id)> on_cell_dclick_;

  wxGridSelectionModes selmode_;
  int interval_;
  int id_;

  void OnChoiceChange(wxCommandEvent &event);
  void OnCellClick(wxGridEvent &e);
  void OnCellDClick(wxGridEvent &e);
};

}
}
}

#endif // _TMS_COMMON_WIDGET__CONTRAPTION_GRID_HPP_
