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
#include "dlg_contact_person.hpp"
#include "dlg_check_column.hpp"
#include "companies_frame.hpp"
#include "dlg_user.hpp"
#include "incomings_frame.hpp"
#include "frames_collection.hpp"

#ifdef FindWindow // MSW workaround
  #include <wx/msw/winundef.h>
#endif //FindWindow

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
  FramesCollection::contact_persons_frame = new DlgContactPerson(this);

  FramesCollection::companies_frame = new CompaniesFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::companies_frame, this,
				  _T("CompaniesFrame"));
  FramesCollection::companies_frame->Init();

  FramesCollection::dlg_user = new DlgUser(this);

  FramesCollection::dlg_check_column = new DlgCheckColumn(this);
  
  FramesCollection::incomings_frame = new IncomingsFrame();
  wxXmlResource::Get()->LoadFrame(FramesCollection::incomings_frame, this,
				  _T("IncomingsFrame"));
  FramesCollection::incomings_frame->Init();

  FramesCollection::frm_grid = this;  
}

void FrmGrid::InitBooks() {
  grid_books_ = new ContraptionGrid(this, wxID_ANY);
  choice_book_ = XRCCTRL(*this, "chcBook", wxChoice);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("chcBook binded"));
  choice_book_->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        (wxObjectEventFunction)&FrmGrid::OnBookSelect,
                        0, this);
  button_add_in_book_ = XRCCTRL(*this, "btnBookAdd", wxButton);    
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("btnBookAdd binded"));
  
  InitIncomingsTable();
  ActivateIncomingsTable();
  wxXmlResource::Get()->AttachUnknownControl("cgrBook", (wxWindow *)grid_books_);
  grid_books_->Connect(wxEVT_GRID_LABEL_RIGHT_CLICK,
                       (wxObjectEventFunction)&FrmGrid::OnBooksLabelRightClick, 0, this);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Books initialized"));
}

void FrmGrid::InitAdmin() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Loading admin page"));
  grid_admin_ = new ContraptionGrid(this, wxID_ANY);
  choice_admin_ = XRCCTRL(*this, "chcAdmin", wxChoice);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("chcAdmin binded"));

  button_add_in_admin_ = XRCCTRL(*this, "btnAdminAdd", wxButton);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("btnAdminAdd binded"));
    
  choice_admin_->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                         (wxObjectEventFunction)&FrmGrid::OnAdminSelect,
                         0, this);
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("btnAdminUpdate binded"));
    
  XRCCTRL(*this, "btnAdminUpdate", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                                      (wxObjectEventFunction)&FrmGrid::OnPatchClick,
                                                      0, this);
  InitUsersTable();
  ActivateUsersTable();
  wxXmlResource::Get()->AttachUnknownControl("cgrAdmin", (wxWindow *)grid_admin_);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Admin initialized"));
}

void FrmGrid::InitCatalogs() {
  grid_catalogs_ = new ContraptionGrid(this, wxID_ANY);
  choice_catalog_ = XRCCTRL(*this, "chcCatalog", wxChoice);
  button_add_in_catalog_ = XRCCTRL(*this, "btnCatalogAdd", wxButton);
  wxXmlResource::Get()->AttachUnknownControl("cgrCatalog", (wxWindow *)grid_catalogs_);
  choice_catalog_->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                           (wxObjectEventFunction)&FrmGrid::OnCatalogSelect,
                           0, this);
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
  
  wxNotebook* notebook = XRCCTRL(*this, "nbMain", wxNotebook);

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

void FrmGrid::ActivateContactPersonsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Activating contact persons table"));  
  button_add_in_catalog_->Show(false);
  grid_catalogs_->ResetColPos();
  grid_catalogs_->SetTable(table_contact_persons_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellDClick(boost::bind(&FrmGrid::OnContactPersonsCellDClick,
					      this, _1, _2));  
  button_add_in_catalog_->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED);
  button_add_in_catalog_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                  (wxObjectEventFunction)&FrmGrid::OnAddInContactPersonClick,
                                  0, this);
  Layout();
}

void FrmGrid::OnContactPersonsCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("OnContactPersonsCellDClick"));
  ContraptionArrayP contraptions =
    dynamic_cast<ContraptionGridTableBase*>(grid_catalogs_->GetTable())->
    contraptions();
  FramesCollection::contact_persons_frame->SetUpValues(contraption, contraptions, false);
  FramesCollection::contact_persons_frame->ShowModal();  
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
    FramesCollection::contact_persons_frame->ShowModal();
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
  DeactivateCatalogs();
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
  DeactivateBooks();
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

void FrmGrid::OnAdminSelect(wxCommandEvent& event) {
  DeactivateAdmin();
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

void FrmGrid::DeactivateBooks() {
  switch (selected_admin_id_) {
    case 0:
      DeactivateIncomingsTable();
      break;
    default:
      break;
  }
}

void FrmGrid::OnBooksLabelRightClick(wxGridEvent &e) {
  FramesCollection::dlg_check_column->SetUpValues(grid_books_, Options::incoming_column_layout, Options::set_incoming_column_layout);
  wxPoint position = e.GetPosition();
  wxWindow *win= grid_books_;
  while (win) {
    position += win->GetPosition();
    win = win->GetParent();
  }
  FramesCollection::dlg_check_column->Move(position);
  FramesCollection::dlg_check_column->Show(true);
}

void FrmGrid::DeactivateAdmin() {
}

void FrmGrid::DeactivateCatalogs() {
}

void FrmGrid::OnClose(wxCloseEvent& event) {
  DeactivateBooks();
  DeactivateAdmin();
  DeactivateCatalogs();
  Options::Save();
  event.Skip();
}

}
}
