#include "string_validator.hpp"
#include "wx/textctrl.h"
namespace tms {
namespace common {
namespace widget {
namespace validators {
StringValidator::StringValidator(GetterFunction getter, SetterFunction setter) :
    getter_(getter),
    setter_(setter) {}

StringValidator::StringValidator(const StringValidator &validator) :
    getter_(validator.getter_),
    setter_(validator.setter_),
    wxTextValidator(validator) {}


bool StringValidator::TransferToWindow() {
  std::string value = getter_();
  wxTextEntry * const text = GetTextEntry();
  if ( !text )
    return false;
  text->SetValue(value);
  return true;
}

bool StringValidator::TransferFromWindow() {
  wxTextEntry * const text = GetTextEntry();
  if ( !text )
    return false;
  
  setter_(text->GetValue().ToStdString());
  return true;
}

wxObject* StringValidator::Clone() const {
  return new StringValidator(*this);
}

}
}
}
}
