#include "has_one_validator.hpp"
// wx
#include <widget/contraption_choice.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

using namespace contraption;

HasOneValidator::HasOneValidator(GetterFunction getter,
                                 SetterFunction setter,
                                 ContraptionArrayP contraptions) :
    getter_(getter),
    setter_(setter),
    contraptions_(contraptions) {
}

HasOneValidator::HasOneValidator(const HasOneValidator &validator) :
    getter_(validator.getter_),
    setter_(validator.setter_),
    contraptions_(validator.contraptions_) {
}

bool HasOneValidator::TransferToWindow() {
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

bool HasOneValidator::TransferFromWindow() {
  wxWindow *validator_window = GetWindow();
  {
    ContraptionChoice *control = dynamic_cast<ContraptionChoice*>(validator_window);
    if (control) {
      setter_(control->GetSelection());
    }
  }
  return false;
}

bool HasOneValidator::Validate(wxWindow * WXUNUSED(parent)) {
  return true;
}

wxObject* HasOneValidator::Clone() const {
  return new HasOneValidator(*this);
}


}
}
}
}
