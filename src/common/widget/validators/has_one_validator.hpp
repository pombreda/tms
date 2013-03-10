#ifndef _TMS_COMMON_WIDGET_VALIDATORS__HAS_ONE_VALIDATOR_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__HAS_ONE_VALIDATOR_HPP_
// wx
#include <wx/validate.h>
// common
#include <contraption/contraption_array.hpp>
#include <contraption/contraption.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

class HasOneValidator : public wxValidator {
 public:
  typedef boost::function<contraption::ContraptionP()> GetterFunction;
  typedef boost::function<void (contraption::ContraptionP)> SetterFunction;
  typedef boost::function<contraption::ContraptionArrayP()> 
                                                  ContraptionArrayGenerator;
  typedef boost::function<contraption::ContraptionP()> 
                                                  ContraptionFactory;


  HasOneValidator(GetterFunction getter, SetterFunction setter, 
                  ContraptionArrayGenerator generator,
                  ContraptionFactory contraption_factory_);
  HasOneValidator(const HasOneValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  bool Validate(wxWindow * WXUNUSED(parent));
  wxObject* Clone() const;
 protected:
  GetterFunction getter_;
  SetterFunction setter_;
  ContraptionArrayGenerator generator_;
  ContraptionFactory contraption_factory_;
};

}
}
}
}

#endif // _TMS_COMMON_WIDGET_VALIDATORS__HAS_ONE_VALIDATOR_HPP_
