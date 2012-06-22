#ifndef _GUI_EXCEPTION_REPORT_DIALOG_
#define _GUI_EXCEPTION_REPORT_DIALOG_

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include <wx/dialog.h>
#include <string>

//-----------------------------------------------------------------------------
// Class definition: GuiExceptionReportDialog
//-----------------------------------------------------------------------------

class GuiExceptionReportDialog : public wxDialog
{

public:
  
  // Constructor.
  /*
    \param parent The parent window. Simple constructor.
  */
  explicit GuiExceptionReportDialog( wxWindow* parent = 0 );
  explicit GuiExceptionReportDialog( std::string message, wxWindow* parent = 0 );
  
  // Destructor.
  ~GuiExceptionReportDialog(){};
  
 private:
  
  // Any class wishing to process wxWidgets events must use this macro
  DECLARE_EVENT_TABLE()
};

#endif  //_GUI_EXCEPTION_REPORT_DIALOG_
