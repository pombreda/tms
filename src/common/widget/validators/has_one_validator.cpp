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
                                 ContraptionArrayGenerator generator,
                                 ContraptionFactory contraption_factory) :
    wxValidator(),
    getter_(getter),
    setter_(setter),
    generator_(generator),
    contraption_factory_(contraption_factory) {
}

HasOneValidator::HasOneValidator(const HasOneValidator &validator) :
    wxValidator(),
    getter_(validator.getter_),
    setter_(validator.setter_),
    generator_(validator.generator_),
    contraption_factory_(validator.contraption_factory_) {
}

bool HasOneValidator::TransferToWindow() {
  wxWindow *validator_window = GetWindow();
  {
    ContraptionChoice *control = dynamic_cast<ContraptionChoice*>(validator_window);
    if (control) {
      ContraptionArrayP contraptions = generator_();
      if (contraptions) {
        control->Enable(true);
        control->set_contraptions(generator_());
        control->set_contraption_factory(contraption_factory_);
        return control->ChooseContraption(getter_());
      } else {
        control->Clear();
        control->Enable(false);
        return true;
      }
    }
  }
  return false;
}

bool HasOneValidator::TransferFromWindow() {
  wxWindow *validator_window = GetWindow();
  {
    ContraptionChoice *control = dynamic_cast<ContraptionChoice*>(validator_window);
    if (control) {
      if (control->IsEnabled()) {
        setter_(control->GetSelection());
        return true;
      } else {
        setter_(ContraptionP());
        return true;
      }
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
