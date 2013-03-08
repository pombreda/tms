#include "hide_if_validator.hpp"
// wx
#include <wx/window.h>

namespace tms {
namespace common {
namespace widget {
namespace validators {

HideIfValidator::HideIfValidator(const ConditionFunction &condition) :
    wxValidator(),
    condition_(condition) { }

HideIfValidator::HideIfValidator(const HideIfValidator &other) :
    wxValidator(),
    condition_(other.condition_) { }

bool HideIfValidator::TransferToWindow() {
  wxWindow *validator_window = GetWindow();
  if (!validator_window) {
    return false;
  }
  if (condition_()) {
    validator_window->Show(false);
  } else {
    validator_window->Show(true);
  }
  return true;
}

bool HideIfValidator::TransferFromWindow() {
  return true;
}

wxObject* HideIfValidator::Clone() const {
  return new HideIfValidator(*this);
}

bool HideIfValidator::Validate(wxWindow * WXUNUSED(parent)) {
  return true;
}

}
}
}
}
