#ifndef _TMS_CLIENT__DLG_CHECK_COLUMN_HPP_
#define _TMS_CLIENT__DLG_CHECK_COLUMN_HPP_

// wxWidgets
#include <wx/dialog.h>
#include <wx/xrc/xmlres.h>
#include <wx/grid.h>
#include <wx/checklst.h>
// boost
#include <boost/function.hpp>
// std
#include <vector>
#include <client/options.hpp>

namespace tms {
namespace client {

class DlgCheckColumn : public wxDialog {
 public:
  typedef boost::function<ColumnLayout(std::string)> LayoutGetter;
  typedef boost::function<void(std::string, ColumnLayout)> LayoutSetter;
  DlgCheckColumn(wxWindow *parent);
  virtual ~DlgCheckColumn();
  void SetUpValues(wxGrid *grid,
                   LayoutGetter layout_getter,
                   LayoutSetter layout_setter);  
 private:
  void Init();
  DlgCheckColumn& operator=(const DlgCheckColumn&);

  wxGrid *grid_;
  wxCheckListBox *check_list_;
  LayoutSetter layout_setter_;
  LayoutGetter layout_getter_;
  void OnItemChecked(wxCommandEvent& event);
  void OnActivate(wxActivateEvent& event);
  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__DLG_CHECK_COLUMN_HPP_
