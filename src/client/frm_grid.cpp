#include "frm_grid.hpp"
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
// frames
#include "contact_persons_frame.hpp"
#include "companies_frame.hpp"
#include "dlg_user.hpp"
#include "incomings_frame.hpp"
#include "frames_collection.hpp"

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

BEGIN_EVENT_TABLE(FrmGrid,wxFrame)
END_EVENT_TABLE()


FrmGrid::FrmGrid() :
    wxFrame(),
    grid_books_(), choice_book_(), selected_book_id_(0),
    grid_catalogs_(), choice_catalog_(), selected_catalog_id_(0),
    table_contact_persons_() {
  wxXmlResource::Get()->LoadFrame(this, 
                                  NULL, 
                                  WStringFromUTF8String("frmGrid"));
}

FrmGrid::~FrmGrid() {
}

void FrmGrid::InitDialogs() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing dialogs"));
  FramesCollection::contact_persons_frame = new ContactPersonsFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::contact_persons_frame, this,
				  _T("ContactPersonsFrame"));
  FramesCollection::contact_persons_frame->Init();

  FramesCollection::companies_frame = new CompaniesFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::companies_frame, this,
				  _T("CompaniesFrame"));
  FramesCollection::companies_frame->Init();

  FramesCollection::dlg_user = new DlgUser();

  FramesCollection::incomings_frame = new IncomingsFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::incomings_frame, this,
				  _T("IncomingsFrame"));
  FramesCollection::incomings_frame->Init();

  FramesCollection::frm_grid = this;  
}

void FrmGrid::InitBooks() {
  grid_books_ = new ContraptionGrid(this, wxID_ANY);
  choice_book_ = (wxChoice *)FindWindowByName("chcBook");
  Connect(XRCID("chcBook"), wxEVT_COMMAND_CHOICE_SELECTED,
	  (wxObjectEventFunction)&FrmGrid::OnBookSelect);
  button_add_in_book_ = (wxButton*) FindWindowByName("btnBookAdd");
    

  InitIncomingsTable();
  ActivateIncomingsTable();
  wxXmlResource::Get()->AttachUnknownControl("cgrBook", (wxWindow *)grid_books_);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Books initialized"));
}

void FrmGrid::InitAdmin() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Loading admin page"));
  grid_admin_ = new ContraptionGrid(this, wxID_ANY);
  choice_admin_ = (wxChoice*) FindWindowByName("chcAdmin");
  button_add_in_admin_ = (wxButton*) FindWindowByName("btnAdminAdd");
  Connect(XRCID("chcAdmin"), wxEVT_COMMAND_CHOICE_SELECTED,
          (wxObjectEventFunction)&FrmGrid::OnAdminSelect);
  Connect(XRCID("btnAdminUpdate"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&FrmGrid::OnPatchClick);
  InitUsersTable();
  ActivateUsersTable();
  wxXmlResource::Get()->AttachUnknownControl("cgrAdmin", (wxWindow *)grid_admin_);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Admin initialized"));
}

void FrmGrid::InitCatalogs() {
  grid_catalogs_ = new ContraptionGrid(this, wxID_ANY);
  choice_catalog_ = (wxChoice*) FindWindowByName("chcCatalog");
  button_add_in_catalog_ = (wxButton*) FindWindowByName("btnCatalogAdd");
  wxXmlResource::Get()->AttachUnknownControl("cgrCatalog", (wxWindow *)grid_catalogs_);
  Connect(XRCID("chcCatalog"), wxEVT_COMMAND_CHOICE_SELECTED,
          (wxObjectEventFunction)&FrmGrid::OnCatalogSelect);
  InitContactPersonsTable();
  InitCompaniesTable();
  ActivateContactPersonsTable();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Catalogs initialized"));
  
}

void FrmGrid::Init() {
  Bind(wxEVT_CLOSE_WINDOW, &FrmGrid::OnClose, this);
  InitDialogs();
  PrepareModels();
  
  wxNotebook* notebook = (wxNotebook*) FindWindowByName("nbMain");

  InitBooks();
  
  InitCatalogs();

  if (Options::admin()) {
    InitAdmin();
  } else {
    notebook->RemovePage(2);
  }
  
  Centre();
  Maximize(true);
  
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("FrmGrid initialized"));
}

void FrmGrid::PrepareModels() {
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

void FrmGrid::InitContactPersonsTable() {
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

void FrmGrid::InitIncomingsTable() {
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


void FrmGrid::ActivateCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating companies table"));  
  button_add_in_catalog_->Show(true);
  grid_catalogs_->ResetColPos();
  grid_catalogs_->SetTable(table_companies_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellClick(boost::bind(&FrmGrid::OnCompaniesCellClick,
					     this, _1, _2));
  grid_catalogs_->SetOnCellDClick(boost::bind(&FrmGrid::OnCompaniesCellDClick,
					      this, _1, _2));  

  Disconnect(XRCID("btnCatalogAdd"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("btnCatalogAdd"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&FrmGrid::OnAddInCompanyClick);
  Layout();
}

void FrmGrid::ActivateContactPersonsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating contact persons table"));  
  button_add_in_catalog_->Show(false);
  grid_catalogs_->ResetColPos();
  grid_catalogs_->SetTable(table_contact_persons_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellClick(boost::bind(&FrmGrid::OnContactPersonsCellClick,
					     this, _1, _2));
  grid_catalogs_->SetOnCellDClick(boost::bind(&FrmGrid::OnContactPersonsCellDClick,
					      this, _1, _2));  

  Disconnect(XRCID("btnCatalogAdd"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("btnCatalogAdd"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&FrmGrid::OnAddInContactPersonClick);
  Layout();
}

void FrmGrid::ActivateIncomingsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating incomings table"));
  grid_books_->ResetColPos();
  grid_books_->SetTable(table_incomings_, wxGrid::wxGridSelectRows, 2500);
  grid_books_->SetOnCellClick(boost::bind(&FrmGrid::OnIncomingsCellClick,
					     this, _1, _2));
  grid_books_->SetOnCellDClick(boost::bind(&FrmGrid::OnIncomingsCellDClick,
					      this, _1, _2));  

  Disconnect(XRCID("btnBookAdd"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("btnBookAdd"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&FrmGrid::OnAddInIncomingClick);
}


void FrmGrid::InitUsersTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Users table"));
  std::vector<Column> cols;
  ModelP model = User::GetModel();
  cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));

  table_users_ =
    new ContraptionGridTableBase(model->All(), cols);

}

void FrmGrid::ActivateUsersTable() {
  grid_admin_->ResetColPos();
  grid_admin_->SetTable(table_users_, wxGrid::wxGridSelectRows, 2500);
  Disconnect(XRCID("btnAdminAdd"), wxEVT_COMMAND_BUTTON_CLICKED);
  Connect(XRCID("btnAdminAdd"), wxEVT_COMMAND_BUTTON_CLICKED,
	  (wxObjectEventFunction)&FrmGrid::OnAddInUserClick);
  grid_admin_->SetOnCellClick(boost::bind(&FrmGrid::OnUsersCellClick, this, _1, _2));
  grid_admin_->SetOnCellDClick(boost::bind(&FrmGrid::OnUsersCellDClick, this, _1, _2));
}

void FrmGrid::InitCompaniesTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing Companies table"));
  try {
    std::vector<Column> cols;
    ModelP model = Company::GetModel();
    cols.push_back(Column(model->GetFieldID("ID"), "ID", 30));
    cols.push_back(Column(model->GetFieldID("short_name"), "Название", 150));
    cols.push_back(Column(model->GetFieldID("gruppa"), "Группа", 100));
    cols.push_back(Column(model->GetFieldID("contact_name"), "Контактное лицо", 120));
    cols.push_back(Column(model->GetFieldID("contact_email"), "Контактный e-mail", 130));
    cols.push_back(Column(model->GetFieldID("contact_phone"), "Контактный телефон", 150));
    table_companies_ =
      new ContraptionGridTableBase(model->All(), cols);

  } catch (GUIException &e) {
    Report(e);
  }
}

void FrmGrid::OnContactPersonsCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void FrmGrid::OnContactPersonsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnContactPersonsCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  FramesCollection::contact_persons_frame->SetUpValues(contraption, contraptions, false);
  FramesCollection::contact_persons_frame->Show(true);  
}

void FrmGrid::OnCompaniesCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void FrmGrid::OnCompaniesCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnCompaniesCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  FramesCollection::companies_frame->SetUpValues(contraption, contraptions);
  FramesCollection::companies_frame->Show(true);  
}

void FrmGrid::OnIncomingsCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void FrmGrid::OnIncomingsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnIncomingsCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_books_->GetTable())->
    contraptions();
  FramesCollection::incomings_frame->SetUpValues(contraption, contraptions);
  FramesCollection::incomings_frame->Show(true);  
}


void FrmGrid::OnUsersCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void FrmGrid::OnUsersCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnAdminCellDClick"));
  try {
    ContraptionArrayP contraptions =
      dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
      contraptions();
    FramesCollection::dlg_user->SetUpValues(contraption, contraptions);
    FramesCollection::dlg_user->Show(true);
  } catch (GUIException &e) {
    Report(e);
  }
}


void FrmGrid::OnAddInContactPersonClick(wxCommandEvent& WXUNUSED(event)) {
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

void FrmGrid::OnAddInCompanyClick(wxCommandEvent& WXUNUSED(event)) {
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


void FrmGrid::OnAddInIncomingClick(wxCommandEvent& WXUNUSED(event)) {
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


void FrmGrid::OnPatchClick(wxCommandEvent& event) {
  wxFileDialog *fd = new wxFileDialog(this, "Select a patch to install", "", "", "*.patch");
  if (fd->ShowModal() == wxID_OK) {
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
}


void FrmGrid::OnCatalogSelect(wxCommandEvent& event) {
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

void FrmGrid::OnBookSelect(wxCommandEvent& event) {
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


void FrmGrid::OnAddInUserClick(wxCommandEvent& WXUNUSED(event)) {
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_admin_->GetTable())->
    contraptions();
  ContraptionP contraption = contraptions->model()->New();
  FramesCollection::dlg_user->SetUpValues(contraption, contraptions);
  FramesCollection::dlg_user->Show(true);
}

void FrmGrid::OnAdminSelect(wxCommandEvent& event) {
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


void FrmGrid::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

void FrmGrid::OnClose(wxCloseEvent& event) {
  
  Options::Save();
  event.Skip();
}


}
}
