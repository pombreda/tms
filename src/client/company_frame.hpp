#ifndef _TMS_CLIENT__COMPANIES_FRAME_HPP_
#define _TMS_CLIENT__COMPANIES_FRAME_HPP_

// wxWidgets
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/xrc/xmlres.h>
// boost
#include <boost/lexical_cast.hpp>
// std
#include <string>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>

namespace tms {
namespace client {

class CompaniesFrame : public wxFrame {
 public:
  CompaniesFrame() :
      wxFrame(), contraption_(), contraptions_() {}
  virtual ~CompaniesFrame();
  void Init();
  void SetUpValues(tms::common::contraption::ContraptionP contraption,
                   tms::common::contraption::ContraptionArrayP contraptions);

 private:
  CompaniesFrame(const CompaniesFrame&);
  CompaniesFrame& operator=(const CompaniesFrame&);


  tms::common::contraption::ContraptionP contraption_;
  tms::common::contraption::ContraptionArrayP contraptions_;

  void OnSaveClick(wxCommandEvent& WXUNUSED(event));
  void OnDeleteClick(wxCommandEvent& WXUNUSED(event));
  void OnExitClick(wxCommandEvent& WXUNUSED(event));
  void OnTryClose(wxCloseEvent& event);

  DECLARE_EVENT_TABLE()
};

}
}

#endif // _TMS_CLIENT__COMPANIES_FRAME_HPP_
