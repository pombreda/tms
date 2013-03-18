#ifndef _TMS_COMMON_WIDGET_VALIDATORS__SUGGEST_VALIDATOR_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__SUGGEST_VALIDATOR_HPP_

// wx
#include <wx/validate.h>
// boost
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
// std
#include <vector>
#include <string>

namespace tms {
namespace common {
namespace widget {
namespace validators {

class SuggestValidator : public wxValidator {
 public:
  typedef boost::function< std::vector<std::string> ()> SuggestProvider;
  SuggestValidator(SuggestProvider suggest, const wxValidator &validator);
  SuggestValidator(const SuggestValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  bool Validate(wxWindow* window);
  wxObject* Clone() const;
 protected:
  SuggestProvider suggest_;
  boost::scoped_ptr<wxValidator> validator_;
};

}
}
}
}

#endif // _TMS_COMMON_WIDGET_VALIDATORS__SUGGEST_VALIDATOR_HPP_
