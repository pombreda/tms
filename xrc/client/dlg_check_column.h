///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dlg_check_column__
#define __dlg_check_column__

#include <wx/string.h>
#include <wx/checklst.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class dlgCheckColumn
///////////////////////////////////////////////////////////////////////////////
class dlgCheckColumn : public wxDialog 
{
	private:
	
	protected:
		wxCheckListBox* chlColumns;
	
	public:
		
		dlgCheckColumn( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxSYSTEM_MENU );
		~dlgCheckColumn();
	
};

#endif //__dlg_check_column__
