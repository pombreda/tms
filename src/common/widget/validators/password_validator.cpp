#include <protocol/crypto.hpp>
#include "password_validator.hpp"
// wx
#include "wx/textctrl.h"
// boost
#include <boost/bind.hpp>
#include <iostream>
using namespace tms::common::protocol;
namespace tms {
namespace common {
namespace widget {
namespace validators {

std::string PasswordValidator::Getter() {
  return "*****";
}

void PasswordValidator::Setter(std::string val) {
  val_ = val;
}

PasswordValidator::PasswordValidator(SetterFunction setter) :
    StringValidator(boost::bind(&PasswordValidator::Getter, this), 
                    boost::bind(&PasswordValidator::Setter, this, _1)),
    setter_(setter) {}

PasswordValidator::PasswordValidator(const PasswordValidator &validator) :
    StringValidator(boost::bind(&PasswordValidator::Getter, this), 
                    boost::bind(&PasswordValidator::Setter, this, _1)),
    setter_(validator.setter_) {}

bool PasswordValidator::TransferFromWindow() {
  if (StringValidator::TransferFromWindow()) {
    if (val_ != "*****") {
      setter_(sha256(val_));
    }
    return true;
  }
  return false;
}

wxObject* PasswordValidator::Clone() const {
  return new PasswordValidator(*this);
}

}
}
}
}
