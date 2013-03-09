#include "has_many_validator.hpp"
// wx
#include <widget/contraption_choice.hpp>

namespace tms {
namespace common {
namespace widget {

using namespace contraption;

HasManyValidator::HasManyValidator(GetterFunction getter,
                                   SetterFunction setter,
                                   ContraptionArrayP contraptions) :
    getter_(getter),
    setter_(setter),
    contraptions_(contraptions) {
}

HasManyValidator(const HasManyValidator &validator) :
    getter_(validator.getter_),
    setter_(validator.setter_),
    contraptions_(validator.contraptions_) {
}

bool HasManyValidator::TransferToWindow() {
  wxWindow *validator_window = GetWindow();
  {
    ContraptionChoice *control = dynamic_cast<ContraptionChoice*>(validator_window);
    if (control) {
      control->set_contraptions(contraptions_);
      return control->ChooseContraption(getter_());
    }
  }
  return false;
}

bool HasManyValidator::TransferFromWindow() {
  wxWindow *validator_window = GetWindow();
  {
    ContraptionChoice *control = dynamic_cast<ContraptionChoice*>(validator_window);
    if (control) {
      setter_(control->GetSelection());
    }
  }
  return false;
}

bool HasManyValidator::Validate() {
  return true;
}

wxObject* HasManyValidator::Clone() const {
  return new HasManyValidator(*this);
}


}
}
}
