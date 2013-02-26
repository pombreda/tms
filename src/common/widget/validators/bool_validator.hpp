#ifndef _TMS_COMMON_WIDGET_VALIDATORS__BOOL_VALIDATOR_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__BOOL_VALIDATOR_HPP_
// std
#include <string>
// boost
#include <boost/function.hpp>
// wx
#include <wx/validate.h>

namespace tms {
namespace common {
namespace widget {
namespace validators {

class BoolValidator : public wxValidator {
 public:
  typedef boost::function<bool ()> GetterFunction;
  typedef boost::function<void (bool)> SetterFunction;
  BoolValidator(GetterFunction getter, SetterFunction setter);
  BoolValidator(const BoolValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
bool Validate(wxWindow * WXUNUSED(parent));
  wxObject* Clone() const;
 protected:
  GetterFunction getter_;
  SetterFunction setter_;
};

}
}
}
}
    

#endif // _TMS_COMMON_WIDGET_VALIDATORS__STRING_VALIDATOR_HPP_
