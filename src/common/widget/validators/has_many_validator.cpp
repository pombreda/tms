#include "has_many_validator.hpp"
// wx
#include <widget/contraption_grid.hpp>
#include <widget/contraption_choice.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

using namespace contraption;

HasManyValidator::HasManyValidator(GetterFunction getter, 
                                   ContraptionFactory contraption_factory) :
    getter_(getter),
    contraption_factory_(contraption_factory) {
}

HasManyValidator::HasManyValidator(const HasManyValidator &validator) :
    wxValidator(),
    getter_(validator.getter_),
    contraption_factory_(validator.contraption_factory_) {
}

bool HasManyValidator::TransferToWindow() {
  wxWindow *validator_window = GetWindow();
  {
    ContraptionChoice *control = dynamic_cast<ContraptionChoice*>(validator_window);
    if (control) {
      control->set_contraptions(getter_());
      return true;
    }
  }

  {
    ContraptionGrid *control = dynamic_cast<ContraptionGrid*>(validator_window);
    if (control) {
      ContraptionGridTableBase *table_base = control->table_base();
      table_base->set_contraptions(getter_());
      table_base->set_contraption_factory(contraption_factory_);
      return true;
    }
  }
  return false;
}


bool HasManyValidator::TransferFromWindow() {
  return true;
}

bool HasManyValidator::Validate(wxWindow * WXUNUSED(parent)) {
  return true;
}

wxObject* HasManyValidator::Clone() const {
  return new HasManyValidator(*this);
}

}
}
}
}
