#ifndef _TMS_COMMON_WIDGET__DLG_CHECK_COLUMN_HPP_
#define _TMS_COMMON_WIDGET__DLG_CHECK_COLUMN_HPP_

// wxWidgets
#include <wx/popupwin.h>
#include <wx/grid.h>
#include <wx/checklst.h>
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>

// boost
#include <boost/function.hpp>
// std
#include <vector>
#include <string>
// log4cplus
#include <log4cplus/logger.h>

namespace tms {
namespace common {
namespace widget {

class ContraptionGrid;

class DlgCheckColumn : public wxPopupTransientWindow {
 public:
  DlgCheckColumn(wxWindow *parent);
  virtual ~DlgCheckColumn();
  void SetUpValues(ContraptionGrid *grid);  
 private:
  void Init();
  DlgCheckColumn& operator=(const DlgCheckColumn&);
  log4cplus::Logger logger_;
  ContraptionGrid *grid_;
  //wxCheckListBox *check_list_;
  std::vector<wxCheckBox*> check_list_;
  wxPanel *panel_;
  wxBoxSizer *top_sizer_; 
  void OnItemChecked(int id);
};

}
}
}
#endif // _TMS_COMMON_WIDGET__DLG_CHECK_COLUMN_HPP_
