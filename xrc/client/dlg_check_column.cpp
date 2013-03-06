///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dlg_check_column.h"

///////////////////////////////////////////////////////////////////////////

dlgCheckColumn::dlgCheckColumn( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bszMain;
	bszMain = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString chlColumnsChoices;
	chlColumns = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, chlColumnsChoices, 0 );
	bszMain->Add( chlColumns, 1, wxEXPAND, 5 );
	
	this->SetSizer( bszMain );
	this->Layout();
	bszMain->Fit( this );
	
	this->Centre( wxBOTH );
}

dlgCheckColumn::~dlgCheckColumn()
{
}
