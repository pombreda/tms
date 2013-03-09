#ifndef _TMS_COMMON_WIDGET__DLG_CHECK_COLUMN_HPP_
#define _TMS_COMMON_WIDGET__DLG_CHECK_COLUMN_HPP_

// wxWidgets
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/checklst.h>
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

class DlgCheckColumn : public wxDialog {
 public:
  DlgCheckColumn(wxWindow *parent);
  virtual ~DlgCheckColumn();
  void SetUpValues(ContraptionGrid *grid);  
 private:
  void Init();
  DlgCheckColumn& operator=(const DlgCheckColumn&);
  log4cplus::Logger logger_;
  ContraptionGrid *grid_;
  wxCheckListBox *check_list_;  
  void OnItemChecked(wxCommandEvent& event);
  void OnActivate(wxActivateEvent& event);
};

}
}
}
#endif // _TMS_COMMON_WIDGET__DLG_CHECK_COLUMN_HPP_
