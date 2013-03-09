// wx
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
// boost
#include <boost/function.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/contraption.hpp>

namespace tms {
namespace common {
namespace widget {

class ContraptionChoice : public wxPanel {
 public:
  typedef boost::function<std::string(contraption::ContraptionP)> DisplayFunction;
  ContraptionChoice(wxWindow *parent, DisplayFunction display);
  virtual ~ContraptionChoice();
  void set_contraptions(contraption::ContraptionArrayP contraptions) {
    contraptions_ = contraptions;
    model_ = contraptions->model();
  }

  contraption::ContraptionArrayP contraptions() {
    return contraptions_;
  }

  bool ChooseContraption(contraption::ContraptionP contraption);

  contraption::ContraptionP GetSelection();
 private:
  void RefreshList();
  contraption::ContraptionArrayP contraptions_;
  contraption::ModelP model_;
  wxChoice *choice_;
  DisplayFunction display_;
};

}
}
}
