#include "grid_frame.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <widget/contraption_grid.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
#include <protocol/message/patch_server_request.hpp>
// wx
#include <wx/notebook.h>
#include <wx/filedlg.h>
#include <wx/button.h>
// std
#include <fstream>
#include <sstream>
#include "options.hpp"

namespace tms{
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget;
using namespace tms::project::model;
using namespace tms::common::model;
using namespace tms::common::string;
using namespace tms::common;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
END_EVENT_TABLE()

GridFrame::~GridFrame() {
}

void GridFrame::Init() {
  // frames
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing frames"));

  FramesCollection::contact_persons_frame = new ContactPersonsFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::contact_persons_frame, this,
				  _T("ContactPersonsFrame"));
  FramesCollection::contact_persons_frame->Init();

  FramesCollection::companies_frame = new CompaniesFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::companies_frame, this,
				  _T("CompaniesFrame"));
  FramesCollection::companies_frame->Init();

  FramesCollection::users_frame = new UsersFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::users_frame, this,
				  _T("UsersFrame"));
  FramesCollection::users_frame->Init();

  FramesCollection::incomings_frame = new IncomingsFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::incomings_frame, this,
				  _T("IncomingsFrame"));
  FramesCollection::incomings_frame->Init();

  FramesCollection::grid_frame = this;
  // frames done
  PrepareModels();
  wxNotebook* notebook = (wxNotebook*) FindWindowByName("ID_NOTEBOOK_MAIN");
  grid_books_ = new ContraptionGrid(this, wxID_ANY);
  choice_book_ = (wxChoice *)FindWindowByName("ID_CHOICE_BOOK");
  Connect(XRCID("ID_CHOICE_BOOK"), wxEVT_COMMAND_CHOICE_SELECTED,
	  (wxObjectEventFunction)&GridFrame::OnBookSelect);
  button_add_in_book_ = (wxButton*) FindWindowByName("ID_BUTTON_ADD_IN_BOOK");
    

  InitIncomingsTable();
  ActivateIncomingsTable();
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
    choice_admin_ = (wxChoice*) FindWindowByName("ID_CHOICE_ADMIN");
    button_add_in_admin_ = (wxButton*) FindWindowByName("ID_BUTTON_ADD_IN_ADMIN");
    Connect(XRCID("ID_CHOICE_ADMIN"), wxEVT_COMMAND_CHOICE_SELECTED,
	    (wxObjectEventFunction)&GridFrame::OnAdminSelect);
    Connect(XRCID("ID_BUTTON_PATCH"), wxEVT_COMMAND_BUTTON_CLICKED,
	    (wxObjectEventFunction)&GridFrame::OnPatchClick);
    InitUsersTable();
    ActivateUsersTable();
    LOG4CPLUS_INFO(client_logger, 
		   WStringFromUTF8String("Admin initialized"));
    //grid_books_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
    //grid_books_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
    wxXmlResource::Get()->AttachUnknownControl("ID_GRID_ADMIN", (wxWindow *)grid_admin_);
  } else {
    notebook->RemovePage(2);
  }
  //Connect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED,
  //        (wxObjectEventFunction)&GridFrame::OnAddClick);

  grid_catalogs_ = new ContraptionGrid(this, wxID_ANY);
  choice_catalog_ =(wxChoice*) FindWindowByName("ID_CHOICE_CATALOG");
  button_add_in_catalog_ = (wxButton*) FindWindowByName("ID_BUTTON_ADD_IN_CATALOG");
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_CATALOGS", (wxWindow *)grid_catalogs_);
  Connect(XRCID("ID_CHOICE_CATALOG"), wxEVT_COMMAND_CHOICE_SELECTED,
          (wxObjectEventFunction)&GridFrame::OnCatalogSelect);
  InitContactPersonsTable();
  InitCompaniesTable();
  ActivateContactPersonsTable();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Catalogs initialized"));
  Centre();
  GetSizer()->RecalcSizes();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("GridFrame initialized"));
  Maximize(true);
}

void GridFrame::PrepareModels() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Preparing models"));  
  User::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "users")));
  ContactPerson::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "contact_persons")));
  Company::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "companies")));
  Incoming::PrepareModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "incomings")));

}

void GridFrame::InitContactPersonsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing ContactPersons table"));
  try {
    
    std::vector<Column> cols;
    ModelP model = ContactPerson::GetModel();
    cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));
    cols.push_back(Column(model->GetFieldID("company_name"), "Компания", 100));
    cols.push_back(Column(model->GetFieldID("role"), "Должность", 100));
    cols.push_back(Column(model->GetFieldID("email"), "Email", 100));
    cols.push_back(Column(model->GetFieldID("code"), "Код", 50));
    cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 100));
    cols.push_back(Column(model->GetFieldID("fax"), "Факс", 100));
    cols.push_back(Column(model->GetFieldID("note"), "Заметки", 200));
    table_contact_persons_ =
      new ContraptionGridTableBase(model->All(), cols);
    
  } catch (GUIException &e) {
    Report(e);
  }
}

void GridFrame::InitIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Incomings table"));
  try {
    
    std::vector<Column> cols;
    ModelP model = Incoming::GetModel();
    cols.push_back(Column(model->GetFieldID("ID"), "ID", 150));
    cols.push_back(Column(model->GetFieldID("company_name"), "Компания", 100));
    cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 150));
    cols.push_back(Column(model->GetFieldID("contact_email"), "Контактный e-mail", 150));
    cols.push_back(Column(model->GetFieldID("contact_phone"), "Контактный телефон", 150));
    cols.push_back(Column(model->GetFieldID("manager_name"), "Адресат", 100));
    cols.push_back(Column(model->GetFieldID("type_in"), "Тип приема", 100));
    cols.push_back(Column(model->GetFieldID("time_in"), "Время приема", 100));
    cols.push_back(Column(model->GetFieldID("time_out"), "Время передачи", 100));
    table_incomings_ =
      new ContraptionGridTableBase(model->All(), cols);
    
  } catch (GUIException &e) {
    Report(e);
  }
}


void GridFrame::ActivateCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating companies table"));  
  button_add_in_catalog_->Show(true);
  grid_catalogs_->SetTable(table_companies_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellClick(boost::bind(&GridFrame::OnCompaniesCellClick,
					     this, _1, _2));
  grid_catalogs_->SetOnCellDClick(boost::bind(&GridFrame::OnCompaniesCellDClick,
					      this, _1, _2));  

  Disconnect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&GridFrame::OnAddInCompanyClick);
  Layout();
}

void GridFrame::ActivateContactPersonsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating contact persons table"));  
  button_add_in_catalog_->Show(false);
  grid_catalogs_->SetTable(table_contact_persons_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellClick(boost::bind(&GridFrame::OnContactPersonsCellClick,
					     this, _1, _2));
  grid_catalogs_->SetOnCellDClick(boost::bind(&GridFrame::OnContactPersonsCellDClick,
					      this, _1, _2));  

  Disconnect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&GridFrame::OnAddInContactPersonClick);
  Layout();
}

void GridFrame::ActivateIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating incomings table"));  
  grid_books_->SetTable(table_incomings_, wxGrid::wxGridSelectRows, 2500);
  grid_books_->SetOnCellClick(boost::bind(&GridFrame::OnIncomingsCellClick,
					     this, _1, _2));
  grid_books_->SetOnCellDClick(boost::bind(&GridFrame::OnIncomingsCellDClick,
					      this, _1, _2));  

  Disconnect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&GridFrame::OnAddInIncomingClick);
}


void GridFrame::InitUsersTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Users table"));
  std::vector<Column> cols;
  ModelP model = User::GetModel();
  cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));

  table_users_ =
    new ContraptionGridTableBase(model->All(), cols);

}

void GridFrame::ActivateUsersTable() {
  grid_admin_->SetTable(table_users_, wxGrid::wxGridSelectRows, 2500);
  Disconnect(XRCID("ID_BUTTON_ADD_IN_ADMIN"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("ID_BUTTON_ADD_IN_ADMIN"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&GridFrame::OnAddInUserClick);
  grid_admin_->SetOnCellClick(boost::bind(&GridFrame::OnUsersCellClick, this, _1, _2));
  grid_admin_->SetOnCellDClick(boost::bind(&GridFrame::OnUsersCellDClick, this, _1, _2));
}

void GridFrame::InitCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Companies table"));
  try {
    std::vector<Column> cols;
    ModelP model = Company::GetModel();
    cols.push_back(Column(model->GetFieldID("short_name"), "Название", 150));
    cols.push_back(Column(model->GetFieldID("gruppa"), "Группа", 100));
    cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 100));
    cols.push_back(Column(model->GetFieldID("contact_email"), "Контактный e-mail", 100));
    cols.push_back(Column(model->GetFieldID("contact_phone"), "Контактный телефон", 100));
    table_companies_ =
      new ContraptionGridTableBase(model->All(), cols);

  } catch (GUIException &e) {
    Report(e);
  }
}

void GridFrame::OnContactPersonsCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnContactPersonsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnContactPersonsCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  FramesCollection::contact_persons_frame->SetUpValues(contraption, contraptions, false);
  FramesCollection::contact_persons_frame->Show(true);  
}

void GridFrame::OnCompaniesCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnCompaniesCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnCompaniesCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  FramesCollection::companies_frame->SetUpValues(contraption, contraptions);
  FramesCollection::companies_frame->Show(true);  
}

void GridFrame::OnIncomingsCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnIncomingsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnIncomingsCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_books_->GetTable())->
    contraptions();
  FramesCollection::incomings_frame->SetUpValues(contraption, contraptions);
  FramesCollection::incomings_frame->Show(true);  
}


void GridFrame::OnUsersCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnUsersCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAdminCellDClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    FramesCollection::users_frame->SetUpValues(contraption, contraptions);
    FramesCollection::users_frame->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}


void GridFrame::OnAddInContactPersonClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInContactPersonClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    ContraptionP contraption = contraptions->model()->New();
    FramesCollection::contact_persons_frame->SetUpValues(contraption, contraptions, false);
    FramesCollection::contact_persons_frame->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}

void GridFrame::OnAddInCompanyClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInCompanyClick"));
  try {

    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    ContraptionP contraption = contraptions->model()->New();
    FramesCollection::companies_frame->SetUpValues(contraption, contraptions);
    FramesCollection::companies_frame->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}


void GridFrame::OnAddInIncomingClick(wxCommandEvent& WXUNUSED(event)) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAddInIncomingClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_books_->GetTable())->
      contraptions();
    ContraptionP contraption = contraptions->model()->New();
    FramesCollection::incomings_frame->SetUpValues(contraption, contraptions);
    FramesCollection::incomings_frame->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}


void GridFrame::OnPatchClick(wxCommandEvent& event) {
  wxFileDialog *fd = new wxFileDialog(this, "Select a patch to install", "", "", "*.patch");
  fd->ShowModal();
  ostringstream sout;
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Installing patch " + std::string(fd->GetPath().utf8_str().data())));

  ifstream fin(fd->GetPath().utf8_str().data(), ios::in | ios::binary);
  const int bufsize = 10240;
  char buf[bufsize];
  while (!fin.eof()) {
    fin.read(buf, bufsize);
    sout.write(buf, fin.gcount());
  }
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Patch read"));

  PatchServerRequestP request(new PatchServerRequest());
  request->set_patch_archive(sout.str());
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Request created"));

  ClientP client = Options::client();
  client->EvalRequest(*request);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String(std::string(fd->GetPath().utf8_str().data()) + " installed"));

  OnExitClick(event);
}


void GridFrame::OnCatalogSelect(wxCommandEvent& event) {
  switch (event.GetSelection()) {
    case 0:
      selected_catalog_id_ = event.GetSelection();
      ActivateContactPersonsTable();
      break;
    case 1:
      selected_catalog_id_ = event.GetSelection();
      ActivateCompaniesTable();
      break;
    default:
      wxMessageDialog mes(this,
                          _T("Данный каталог не поддерживается.\nПожалуйста, обратитесь к разработчику."));
      mes.ShowModal();
      choice_catalog_->SetSelection(selected_catalog_id_);
      break;
  }
}

void GridFrame::OnBookSelect(wxCommandEvent& event) {
  switch (event.GetSelection()) {
    case 0:
      selected_book_id_ = event.GetSelection();
      ActivateIncomingsTable();
      break;
    default:
      wxMessageDialog mes(this,
                          _T("Данный журнал не поддерживается.\nПожалуйста, обратитесь к разработчику."));
      mes.ShowModal();
      choice_book_->SetSelection(selected_book_id_);
      break;
  }
}


void GridFrame::OnAddInUserClick(wxCommandEvent& WXUNUSED(event)) {
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_admin_->GetTable())->
    contraptions();
  ContraptionP contraption = contraptions->model()->New();
  FramesCollection::users_frame->SetUpValues(contraption, contraptions);
  FramesCollection::users_frame->Show(true);
}

void GridFrame::OnAdminSelect(wxCommandEvent& event) {
  switch (event.GetSelection()) {
    case 0:
      selected_admin_id_ = event.GetSelection();
      ActivateUsersTable();
      break;
    default:
      wxMessageDialog mes(this,
                          _T("Данный каталог не поддерживается.\nПожалуйста, обратитесь к разработчику."));
      mes.ShowModal();
      choice_admin_->SetSelection(selected_admin_id_);
      break;
  }
}


void GridFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
