#ifndef _TMS_COMMON_WIDGET_VALIDATORS__PASSWORD_VALIDATOR_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__PASSWORD_VALIDATOR_HPP_
// std
#include <string>
// boost
#include <boost/function.hpp>
// wx
#include <widget/validators/string_validator.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

class PasswordValidator : public StringValidator {
 public:
  PasswordValidator(SetterFunction setter);
  PasswordValidator(const PasswordValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  wxObject* Clone() const;
};

}
}
}
}
    

#endif // _TMS_COMMON_WIDGET_VALIDATORS__PASSWORD_VALIDATOR_HPP_
