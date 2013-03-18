#include "suggest_validator.hpp"
// wx
#include <wx/combobox.h>
namespace tms {
namespace common {
namespace widget {
namespace validators {
using namespace std;

SuggestValidator::SuggestValidator(SuggestProvider suggest, 
                                   const wxValidator &validator) :
    wxValidator(),
    suggest_(suggest),
    validator_(dynamic_cast<wxValidator*>(validator.Clone())) {
  assert(validator_);
}

SuggestValidator::SuggestValidator(const SuggestValidator &validator) :
    wxValidator(),
    suggest_(validator.suggest_),
    validator_(dynamic_cast<wxValidator*>(validator.validator_->Clone())) {
  assert(validator_);
}

wxObject* SuggestValidator::Clone() const {
  return new SuggestValidator(*this);
}

bool SuggestValidator::TransferToWindow()  {
  wxWindow *validator_window = GetWindow();
  if (!validator_window) {
    return false;
  }

  vector<string> suggest = suggest_();
  {
    wxComboBox *control = dynamic_cast<wxComboBox*>(validator_window);
    if (control) {
      control->Clear();
      for (size_t i = 0; i < suggest.size(); ++i) {
        control->Append(wxString::FromUTF8(suggest[i].c_str()));
      }
    }
  }

  validator_->SetWindow(validator_window);
  return validator_->TransferToWindow();
}

bool SuggestValidator::TransferFromWindow() {
  validator_->SetWindow(GetWindow());
  return validator_->TransferFromWindow();
}

bool SuggestValidator::Validate(wxWindow* window) {
  validator_->SetWindow(GetWindow());
  return validator_->Validate(window);  
}

}
}
}
}
