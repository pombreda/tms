#include "bool_validator.hpp"

// wx
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/tglbtn.h>

namespace tms {
namespace common {
namespace widget {
namespace validators {
BoolValidator::BoolValidator(GetterFunction getter, SetterFunction setter) :
    getter_(getter),
    setter_(setter) {}

BoolValidator::BoolValidator(const BoolValidator &validator) :
    getter_(validator.getter_),
    setter_(validator.setter_),
    wxValidator() {}


bool BoolValidator::TransferToWindow() {
  wxWindow *validator_window = GetWindow();
  if ( !validator_window )
    return false;

  {
    wxCheckBox *control = dynamic_cast<wxCheckBox*>(validator_window);
    if (control) {
      control->SetValue(getter_());
      return true;
    }
  }

  {
    wxRadioButton *control = dynamic_cast<wxRadioButton*>(validator_window);
    if (control) {
      control->SetValue(getter_());
      return true;
    }
  }


  {
    wxToggleButton *control = dynamic_cast<wxToggleButton*>(validator_window);
    if (control) {
      control->SetValue(getter_());
      return true;
    }
  }

  {
    wxBitmapToggleButton *control = dynamic_cast<wxBitmapToggleButton*>(validator_window);
    if (control) {
      control->SetValue(getter_());
      return true;
    }
  }

  return false;
}

bool BoolValidator::TransferFromWindow() {
    wxWindow *validator_window = GetWindow();
  if ( !validator_window )
    return false;

  {
    wxCheckBox *control = dynamic_cast<wxCheckBox*>(validator_window);
    if (control) {
      setter_(control->GetValue());
      return true;
    }
  }

  {
    wxRadioButton *control = dynamic_cast<wxRadioButton*>(validator_window);
    if (control) {
      setter_(control->GetValue());
      return true;
    }
  }

  {
    wxToggleButton *control = dynamic_cast<wxToggleButton*>(validator_window);
    if (control) {
      setter_(control->GetValue());
      return true;
    }
  }

  {
    wxBitmapToggleButton *control = dynamic_cast<wxBitmapToggleButton*>(validator_window);
    if (control) {
      setter_(control->GetValue());
      return true;
    }
  }

  return false;
}

wxObject* BoolValidator::Clone() const {
  return new BoolValidator(*this);
}


bool BoolValidator::Validate(wxWindow * WXUNUSED(parent)) {
  return true;
}
    
}
}
}
}
