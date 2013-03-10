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
  typedef boost::function<contraption::ContraptionP ()> ContraptionFactory;
  typedef boost::function<contraption::ContraptionArrayP ()> GetterFunction;
  HasManyValidator(GetterFunction getter, ContraptionFactory contraption_factory);
  HasManyValidator(const HasManyValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  bool Validate(wxWindow * WXUNUSED(parent));
  wxObject* Clone() const;
 protected:
  GetterFunction getter_;
  ContraptionFactory contraption_factory_;
};

}
}
}
}

#endif // _TMS_COMMON_WIDGET_VALIDATORS__HAS_MANY_VALIDATOR_HPP_
