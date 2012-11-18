#include "grid_frame.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
// wx
#include <wx/notebook.h>
#include <wx/filedlg.h>


namespace tms{
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget;
using namespace tms::project::model;
using namespace tms::common::model;
using namespace tms::common::string;
using namespace tms::common;

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
END_EVENT_TABLE()

GridFrame::~GridFrame() {
}

void GridFrame::Init() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing grid frame"));
  PrepareModels();
  InitContactPersonsTable();
  InitUsersTable();
  wxNotebook* notebook = (wxNotebook*) FindWindowByName("ID_NOTEBOOK_MAIN");
  notebook->RemovePage(0);
  grid_books_ = new ContraptionGrid(this, wxID_ANY);
  choice_book_ = (wxChoice *)FindWindowByName("ID_CHOICE_BOOK");
  grid_books_->CreateGrid(5, 5);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Books initialized"));
  //grid_books_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
  //grid_books_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_BOOKS", (wxWindow *)grid_books_);
  //Connect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED,
  //        (wxObjectEventFunction)&GridFrame::OnAddClick);

  if (Options::admin()) {
    LOG4CPLUS_INFO(client_logger, 
		   WStringFromUTF8String("Loading admin page"));
  
    grid_admin_ = new ContraptionGrid(this, wxID_ANY);
    choice_admin_ = (wxChoice *)FindWindowByName("ID_CHOICE_ADMIN");
    grid_admin_->SetTable(table_users_, wxGrid::wxGridSelectRows, 2500);
    Connect(XRCID("ID_BUTTON_ADD_IN_ADMIN"), wxEVT_COMMAND_BUTTON_CLICKED,
	    (wxObjectEventFunction)&GridFrame::OnAddInAdminClick);
    Connect(XRCID("ID_CHOICE_ADMIN"), wxEVT_COMMAND_CHOICE_SELECTED,
	    (wxObjectEventFunction)&GridFrame::OnAdminSelect);
    Connect(XRCID("ID_BUTTON_PATCH"), wxEVT_COMMAND_BUTTON_CLICKED,
	    (wxObjectEventFunction)&GridFrame::OnPatchClick);
    grid_admin_->SetOnCellClick(boost::bind(&GridFrame::OnAdminCellClick, this, _1, _2));
    grid_admin_->SetOnCellDClick(boost::bind(&GridFrame::OnAdminCellDClick, this, _1, _2));
    users_frame_ = new UsersFrame();
    wxXmlResource::Get()->LoadFrame(users_frame_, this,
				    _T("UsersFrame"));
    users_frame_->Init();
    LOG4CPLUS_INFO(client_logger, 
		   WStringFromUTF8String("Admin initialized"));
    //grid_books_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
    //grid_books_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
    wxXmlResource::Get()->AttachUnknownControl("ID_GRID_ADMIN", (wxWindow *)grid_admin_);
  } else {
    notebook->RemovePage(1);
  }
  //Connect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED,
  //        (wxObjectEventFunction)&GridFrame::OnAddClick);

  grid_catalogs_ = new ContraptionGrid(this, wxID_ANY);
  choice_catalog_ =(wxChoice *)FindWindowByName("ID_CHOICE_CATALOG");
  grid_catalogs_->SetTable(table_contact_persons_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellClick(boost::bind(&GridFrame::OnCatalogCellClick, this, _1, _2));
  grid_catalogs_->SetOnCellDClick(boost::bind(&GridFrame::OnCatalogCellDClick, this, _1, _2));
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_CATALOGS", (wxWindow *)grid_catalogs_);
  Connect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddInCatalogClick);
  Connect(XRCID("ID_CHOICE_CATALOG"), wxEVT_COMMAND_CHOICE_SELECTED,
          (wxObjectEventFunction)&GridFrame::OnCatalogSelect);

  contact_persons_frame_ = new ContactPersonsFrame();
  wxXmlResource::Get()->LoadFrame(contact_persons_frame_, this,
                                  _T("ContactPersonsFrame"));
  contact_persons_frame_->Init();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Catalogs initialized"));
  Centre();
  GetSizer()->RecalcSizes();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("GridFrame initialized"));
}

void GridFrame::PrepareModels() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Preparing ContactPersons table"));  
  User::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "users")));
  ContactPerson::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "contact_persons")));
}

void GridFrame::InitContactPersonsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing ContactPersons table"));
  
  std::vector<Column> cols;
  cols.push_back(Column(0, "Имя", 150));
  cols.push_back(Column(1, "Код", 100));
  cols.push_back(Column(2, "Email", 50));
  cols.push_back(Column(4, "Телефон", 100));
  cols.push_back(Column(5, "Факс", 100));
  cols.push_back(Column(3, "Заметки", 200));
  ModelP model = ContactPerson::GetModel();
  table_contact_persons_ =
    new ContraptionGridTableBase(model->All(), cols);
}

void GridFrame::InitUsersTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Users table"));
  std::vector<Column> cols;
  cols.push_back(Column(0, "Имя", 150));
  User::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "users")));
 
  ModelP model = User::GetModel();
  table_users_ =
    new ContraptionGridTableBase(model->All(), cols);
}


void GridFrame::OnCatalogCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnCatalogCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnCatalogCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  switch (selected_catalog_id_) {
    case 0:
      contact_persons_frame_->SetUpValues(contraption, contraptions);
      contact_persons_frame_->Show(true);
      break;
    default:
      break;
  }
}

void GridFrame::OnAdminCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnAdminCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAdminCellDClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    switch (selected_catalog_id_) {
    case 0:
      users_frame_->SetUpValues(contraption, contraptions);
      users_frame_->Show(true);
      break;
    default:
      break;
    }
  } catch (GUIException &e) {
    Report(e);
  }
}


void GridFrame::OnAddInCatalogClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInCatalogClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    ContraptionP contraption = contraptions ->model()->New();
    switch (selected_catalog_id_) {
    case 0:
      contact_persons_frame_->SetUpValues(contraption, contraptions);
      contact_persons_frame_->Show(true);
      break;
    default:
      break;
    }
  } catch (GUIException &e) {
    Report(e);
  }
}

void GridFrame::OnPatchClick(wxCommandEvent& WXUNUSED(event)) {
  wxFileDialog *fd = new wxFileDialog(this, "Select a patch to install", "", "", "*.patch");
  fd->ShowModal();
}


void GridFrame::OnCatalogSelect(wxCommandEvent& event) {
  switch (event.GetSelection()) {
    case 0:
      selected_catalog_id_ = event.GetSelection();
    default:
      wxMessageDialog mes(this,
                          _T("Данный каталог не поддерживается.\nПожалуйста, обратитесь к разработчику."));
      mes.ShowModal();
      choice_catalog_->SetSelection(selected_catalog_id_);
      break;
  }
}

void GridFrame::OnAddInAdminClick(wxCommandEvent& WXUNUSED(event)) {
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_admin_->GetTable())->
    contraptions();
  ContraptionP contraption = contraptions ->model()->New();
  switch (selected_catalog_id_) {
    case 0:
      users_frame_->SetUpValues(contraption, contraptions);
      users_frame_->Show(true);
      break;
    default:
      break;
  }
}

void GridFrame::OnAdminSelect(wxCommandEvent& event) {
  switch (event.GetSelection()) {
    case 0:
      selected_catalog_id_ = event.GetSelection();
    default:
      wxMessageDialog mes(this,
                          _T("Данный каталог не поддерживается.\nПожалуйста, обратитесь к разработчику."));
      mes.ShowModal();
      choice_catalog_->SetSelection(selected_catalog_id_);
      break;
  }
}


void GridFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
