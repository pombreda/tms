#include "string_validator.hpp"
// wx
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/combobox.h>
// std
#include <iostream>
namespace tms {
namespace common {
namespace widget {
namespace validators {
StringValidator::StringValidator(GetterFunction getter, SetterFunction setter) :
    wxValidator(),
    getter_(getter),
    setter_(setter) {}

StringValidator::StringValidator(const StringValidator &validator) :
    wxValidator(),
    getter_(validator.getter_),
    setter_(validator.setter_) {}


bool StringValidator::TransferToWindow() {
  wxString value = wxString::FromUTF8(getter_().c_str());
  wxWindow *validator_window = GetWindow();
  if (!validator_window )
    return false;
  
  {
    wxTextCtrl *control = dynamic_cast<wxTextCtrl*>(validator_window);
    if (control) {
      control->SetValue(value);
      return true;
    }
  }

  {
    wxComboBox *control = dynamic_cast<wxComboBox*>(validator_window);
    if (control) {
      control->SetValue(value);
      return true;
    }
  }

  {
    wxChoice *control = dynamic_cast<wxChoice*>(validator_window);
    if (control) {
      int pos = control->FindString(value);
      control->SetSelection(pos);
      return true;
    }
  }

  return false;
}

bool StringValidator::Validate(wxWindow* /*window*/) {
  return true;
}

bool StringValidator::TransferFromWindow() {
  wxWindow *validator_window = GetWindow();
  
  if (!validator_window )
    return false;
  
  {
    wxTextCtrl *control = dynamic_cast<wxTextCtrl*>(validator_window);
    if (control) {
      setter_(control->GetValue().utf8_str().data());
      return true;
    }
  }

  {
    wxComboBox *control = dynamic_cast<wxComboBox*>(validator_window);
    if (control) {
      setter_(control->GetValue().utf8_str().data());
      return true;
    }
  }

  {
    wxChoice *control = dynamic_cast<wxChoice*>(validator_window);
    if (control) {
      setter_(control->GetStringSelection().utf8_str().data());
      return true;
    }
  }
  return false;
}

wxObject* StringValidator::Clone() const {
  return new StringValidator(*this);
}

}
}
}
}
