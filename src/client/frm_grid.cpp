#include "frm_grid.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>
// common
#include <string/string.hpp>
#include <widget/contraption_grid.hpp>
#include <widget/dlg_check_column.hpp>
#include <gui_exception/gui_exception.hpp>
#include <gui_exception/gui_exception_report.hpp>
#include <protocol/message/patch_server_request.hpp>
// wx
#include <wx/notebook.h>
#include <wx/filedlg.h>
#include <wx/button.h>
#include <wx/app.h>
#include <wx/menu.h>
// std
#include <fstream>
#include <sstream>
#include "options.hpp"
// frames
#include "dlg_contact_person.hpp"
#include "dlg_user.hpp"
#include "dlg_incoming.hpp"
#include "dlg_login.hpp"
#include "dlg_company.hpp"
#include "dlg_subject.hpp"
#include "dlg_addressee.hpp"
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

FrmGrid::FrmGrid() :
    wxFrame() {
  wxXmlResource::Get()->LoadFrame(this, 
                                  NULL, 
                                  WStringFromUTF8String("frmGrid"));
  Init();
  SetTitle(WStringFromUTF8String("TMS"));
}

FrmGrid::~FrmGrid() {
}

void FrmGrid::InitDialogs() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing dialogs"));
  FramesCollection::dlg_contact_person = new DlgContactPerson(this);
  FramesCollection::dlg_user = new DlgUser(this);
  FramesCollection::dlg_company = new DlgCompany(this);
  FramesCollection::dlg_incoming = new DlgIncoming(this);
  FramesCollection::dlg_subject = new DlgSubject(this);
  FramesCollection::dlg_addressee = new DlgAddressee(this);
  FramesCollection::frm_grid = this;  
}

void FrmGrid::InitBooks() {
  grid_books_ = new ContraptionGrid(this, wxID_ANY);
  grid_books_->set_table_choice(XRCCTRL(*this, "chcBook", wxChoice));
  grid_books_->set_add_button(XRCCTRL(*this, "btnBookAdd", wxButton));    
  InitIncomingsTable();
  grid_books_->AddTable(table_incomings_);
  grid_books_->ChooseTable(0);
  wxXmlResource::Get()->AttachUnknownControl("cgrBook", (wxWindow *)grid_books_);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Books initialized"));
}


void FrmGrid::InitCatalogs() {
  grid_catalogs_ = new ContraptionGrid(this, wxID_ANY);
  grid_catalogs_->set_table_choice(XRCCTRL(*this, "chcCatalog", wxChoice));
  grid_catalogs_->set_add_button(XRCCTRL(*this, "btnCatalogAdd", wxButton));
  InitContactPersonsTable();
  InitCompaniesTable();
  grid_catalogs_->AddTable(table_contact_persons_);
  grid_catalogs_->AddTable(table_companies_);
  grid_catalogs_->ChooseTable(0);
  wxXmlResource::Get()->AttachUnknownControl("cgrCatalog", (wxWindow *)grid_catalogs_);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Catalogs initialized"));
  
}


void FrmGrid::InitAdmin() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Loading admin page"));
  grid_admin_ = new ContraptionGrid(this, wxID_ANY);
  grid_admin_->set_add_button(XRCCTRL(*this, "btnAdminAdd", wxButton));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("btnAdminAdd binded"));
  grid_admin_->set_table_choice(XRCCTRL(*this, "chcAdmin", wxChoice));
  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("chcAdmin binded"));
 
    
  XRCCTRL(*this, "btnAdminUpdate", wxButton)->
      Connect(wxEVT_COMMAND_BUTTON_CLICKED,
              (wxObjectEventFunction)&FrmGrid::OnPatchClick,
              0, this);
  XRCCTRL(*this, "btnImportIncoming", wxButton)->
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,
	 boost::bind(&FrmGrid::OnImportIncomingClick, this));

  LOG4CPLUS_DEBUG(client_logger, 
                  WStringFromUTF8String("btnAdminUpdate binded"));

  InitUsersTable();
  InitSubjectsTable();
  InitAddresseesTable();
  grid_admin_->AddTable(table_users_);
  grid_admin_->AddTable(table_subjects_);
  grid_admin_->AddTable(table_addressees_);
  grid_admin_->ChooseTable(0);
  wxXmlResource::Get()->AttachUnknownControl("cgrAdmin", (wxWindow *)grid_admin_);
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Admin initialized"));
}


void FrmGrid::Init() {
  PrepareModels();

  InitDialogs();
  
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
  Connect(XRCID("mitExit"),
	  wxEVT_COMMAND_MENU_SELECTED, 
	  wxCommandEventHandler(FrmGrid::OnExitClick));

  Bind(wxEVT_CLOSE_WINDOW, &FrmGrid::OnClose, this);
  
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
  Subject::PrepareModel(ModelBackendP(
      new ServerModelBackend(Options::client(), "subjects")));
  Addressee::PrepareModel(ModelBackendP(
      new ServerModelBackend(Options::client(), "addressees")));

}

void FrmGrid::InitContactPersonsTable() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Initializing ContactPersons table"));
  try {
    
    std::vector<Column> cols;
    ModelP model = ContactPerson::GetModel();
    cols.push_back(Column(model->GetFieldID("name"), "Имя", 150));
    cols.push_back(Column(model->GetFieldID("company_name"), "Компания", 100));
    cols.push_back(Column(model->GetFieldID("position"), "Должность", 100));
    cols.push_back(Column(model->GetFieldID("email"), "Email", 100));
    cols.push_back(Column(model->GetFieldID("city_code"), "Код", 50));
    cols.push_back(Column(model->GetFieldID("phone"), "Телефон", 100));
    cols.push_back(Column(model->GetFieldID("fax"), "Факс", 100));
    cols.push_back(Column(model->GetFieldID("note"), "Заметки", 200));
    table_contact_persons_ =
        new ContraptionGridTableBase(model->All(), "Контактные лица", cols);
    table_contact_persons_->set_contraption_dialog(FramesCollection::dlg_contact_person);
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

void FrmGrid::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

void FrmGrid::OnClose(wxCloseEvent& event) {
  Options::Save();
  event.Skip();
}

}
}
