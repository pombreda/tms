#include <protocol/crypto.hpp>
#include "password_validator.hpp"
#include "wx/textctrl.h"
using namespace tms::common::protocol;
namespace tms {
namespace common {
namespace widget {
namespace validators {
PasswordValidator::PasswordValidator(SetterFunction setter) :
    StringValidator(GetterFunction(), setter) {}

PasswordValidator::PasswordValidator(const PasswordValidator &validator) :
    StringValidator(validator) {}


bool PasswordValidator::TransferToWindow() {
  std::string value = "*****";
  wxTextEntry * const text = GetTextEntry();
  if ( !text )
    return false;
  text->SetValue(value);
  return true;
}

bool PasswordValidator::TransferFromWindow() {
  wxTextEntry * const text = GetTextEntry();
  if ( !text )
    return false;
  std::string val = text->GetValue().ToStdString();
  if (val != "*****") {
    setter_(sha256(val));
  }
  return true;
}

wxObject* PasswordValidator::Clone() const {
  return new PasswordValidator(*this);
}

}
}
}
}
