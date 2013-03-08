#ifndef _TMS_COMMON_WIDGET_VALIDATORS__HIDE_IF_VALIDATOR_HPP
#define _TMS_COMMON_WIDGET_VALIDATORS__HIDE_IF_VALIDATOR_HPP
// wx
#include <wx/validate.h>
// boost
#include <boost/function.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

class HideIfValidator : public wxValidator {
 public:
  typedef boost::function<bool ()> ConditionFunction;
  HideIfValidator(const ConditionFunction &condition);
  HideIfValidator(const HideIfValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  bool Validate(wxWindow * WXUNUSED(parent));
  wxObject* Clone() const;
 protected:
  ConditionFunction condition_;
};

}
}
}
}

#endif // _TMS_COMMON_WIDGET_VALIDATORS__HIDE_IF_VALIDATOR_HPP
