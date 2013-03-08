#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_DIALOG_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_DIALOG_HPP_

// wxWidget
#include <wx/dialog.h>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>
// log4cplus
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace tms {
namespace common {
namespace widget {

class ContraptionDialog : public wxDialog {
 public:
  ContraptionDialog();
  virtual ~ContraptionDialog();
  void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);
 protected:
  virtual void Init() = 0;
  virtual void Save();
  virtual void Delete();
  virtual void Exit();
  log4cplus::Logger logger_;
  tms::common::contraption::ContraptionP contraption_;
  tms::common::contraption::ContraptionArrayP contraptions_;
 private:
  ContraptionDialog(const ContraptionDialog&);
  ContraptionDialog& operator=(const ContraptionDialog&);
};

}
}
}

#endif // _TMS_COMMON_WIDGET__CONTRAPTION_DIALOG_HPP_
