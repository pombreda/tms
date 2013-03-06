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
    wxTextValidator(validator),
    getter_(validator.getter_),
    setter_(validator.setter_) {}


bool StringValidator::TransferToWindow() {
  std::string value = getter_();
  wxTextEntry * const text = GetTextEntry();
  if ( !text )
    return false;
  text->SetValue(wxString::FromUTF8(value.c_str()));
  return true;
}

bool StringValidator::TransferFromWindow() {
  wxTextEntry * const text = GetTextEntry();
  if ( !text )
    return false;
  
  setter_(text->GetValue().utf8_str().data());
  return true;
}

wxObject* StringValidator::Clone() const {
  return new StringValidator(*this);
}

}
}
}
}
