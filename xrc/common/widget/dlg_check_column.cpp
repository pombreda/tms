///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dlg_check_column.h"

///////////////////////////////////////////////////////////////////////////

pnlCheckColumn::pnlCheckColumn( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bszMain;
	bszMain = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString chlColumnsChoices;
	chlColumns = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, chlColumnsChoices, 0 );
	bszMain->Add( chlColumns, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bszMain );
	this->Layout();
	bszMain->Fit( this );
}

pnlCheckColumn::~pnlCheckColumn()
{
}
