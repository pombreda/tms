//-----------------------------------------------------------------------------
// Standard wxWidgets headers
//-----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// For all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//-----------------------------------------------------------------------------
// Headers of this .cpp file
//-----------------------------------------------------------------------------

#include "gui_exception_report_dialog.hpp"
#include <wx/xrc/xmlres.h>

using namespace std;

//-----------------------------------------------------------------------------
// Event table: connect the events to the handler functions to process them
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(GuiExceptionReportDialog, wxDialog)
END_EVENT_TABLE()

//-----------------------------------------------------------------------------
// Public members
//-----------------------------------------------------------------------------

GuiExceptionReportDialog::GuiExceptionReportDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadDialog(this, parent, _T("gui_exception_report_dialog"));
}

GuiExceptionReportDialog::GuiExceptionReportDialog(string message, wxWindow* parent)
{
  wxXmlResource::Get()->Load(_T("xrc/common/gui_exception/gui_exception_report_dialog.xrc"));
  wxXmlResource::Get()->LoadDialog(this, parent, _T("gui_exception_report_dialog"));
  XRCCTRL(*this, "exception_text_", wxTextCtrl)->SetValue(wxString(message.c_str(), wxConvUTF8));
}
