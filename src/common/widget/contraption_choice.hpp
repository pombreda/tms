#ifndef _TMS_COMMON_WIDGET__CONTRAPTION_CHOICE_HPP_
#define _TMS_COMMON_WIDGET__CONTRAPTION_CHOICE_HPP_
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
#include <widget/contraption_dialog.hpp>

namespace tms {
namespace common {
namespace widget {

class ContraptionChoice : public wxPanel {
 public:
  typedef boost::function<std::string(contraption::ContraptionP)> DisplayFunction;
  typedef boost::function<contraption::ContraptionP()> 
                                                  ContraptionFactory;
  typedef boost::function<void(contraption::ContraptionP contraption)> OnChoice;
  ContraptionChoice(wxWindow *parent, DisplayFunction display);
  virtual ~ContraptionChoice();
  void set_contraptions(contraption::ContraptionArrayP contraptions) {
    contraptions_ = contraptions;
    model_ = contraptions->model();
    contraptions_->SetOnChange(boost::bind
                               (&ContraptionChoice::RefreshList, this));

  }

  void set_contraption_factory(ContraptionFactory contraption_factory) {
    contraption_factory_ = contraption_factory;
  }

  contraption::ContraptionArrayP contraptions() {
    return contraptions_;
  }

  bool ChooseContraption(contraption::ContraptionP contraption);
  void set_contraption_dialog(ContraptionDialog *contraption_dialog) {
    contraption_dialog_ = contraption_dialog;
  }
  
  void Clear();
  void BindOnChoice(OnChoice event_sink);
  void ClearOnChoiceBinds();

  contraption::ContraptionP GetSelection();
 private:
  boost::signal<void(contraption::ContraptionP contraption)> on_choice_;
  void Init();
  void OnChoiceChange(wxCommandEvent &event);
  contraption::ContraptionP DefaultContraptionFactory();
  void RefreshList();
  void ShowContraption();
  void AddContraption();
  contraption::ContraptionArrayP contraptions_;
  ContraptionDialog *contraption_dialog_;  
  contraption::ModelP model_;
  wxChoice *choice_;
  DisplayFunction display_;
  ContraptionFactory contraption_factory_;
};

}
}
}
#endif // _TMS_COMMON_WIDGET__CONTRAPTION_CHOICE_HPP_
