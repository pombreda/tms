///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DLG_CHECK_COLUMN_H__
#define __DLG_CHECK_COLUMN_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/checklst.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class pnlCheckColumn
///////////////////////////////////////////////////////////////////////////////
class pnlCheckColumn : public wxPanel 
{
	private:
	
	protected:
		wxCheckListBox* chlColumns;
	
	public:
		
		pnlCheckColumn( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~pnlCheckColumn();
	
};

#endif //__DLG_CHECK_COLUMN_H__
