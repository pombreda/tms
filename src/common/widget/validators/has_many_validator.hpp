#ifndef _TMS_COMMON_WIDGET_VALIDATORS__HAS_MANY_VALIDATOR_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__HAS_MANY_VALIDATOR_HPP_
// wx
#include <wx/validate.h>
// common
#include <contraption/contraption_array.hpp>
#include <contraption/contraption.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

class HasManyValidator : public wxValidator {
 public:
  typedef boost::function<ContraptionArrayP ()> GetterFunction;
  typedef boost::function<void (ContraptionArrayP)> SetterFunction;
  HasManyValidator(GetterFunction getter, SetterFunction setter, 
                   contraption::ContraptionArrayP contraptions);
  HasManyValidator(const HasManyValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  bool Validate(wxWindow * WXUNUSED(parent));
  wxObject* Clone() const;
 protected:
  GetterFunction getter_;
  SetterFunction setter_;
  contraption::ContraptionArrayP contraptions_;
}

}
}
}
}

#endif // _TMS_COMMON_WIDGET_VALIDATORS__HAS_MANY_VALIDATOR_HPP_
