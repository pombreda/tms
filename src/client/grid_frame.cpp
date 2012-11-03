#include "grid_frame.hpp"

namespace tms{
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget;
using namespace tms::project::model;

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
END_EVENT_TABLE()

GridFrame::~GridFrame() {
}

void GridFrame::Init() {
  InitContactPersonsTable();

  grid_books_ = new ContraptionGrid(this, wxID_ANY);
  choice_book_ = (wxChoice *)FindWindowByName("ID_CHOICE_BOOK");
  grid_books_->CreateGrid(5, 5);
  //grid_books_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
  //grid_books_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_BOOKS", (wxWindow *)grid_books_);
  //Connect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED,
  //        (wxObjectEventFunction)&GridFrame::OnAddClick);

  grid_catalogs_ = new ContraptionGrid(this, wxID_ANY);
  choice_catalog_ = (wxChoice *)FindWindowByName("ID_CHOICE_CATALOG");
  grid_catalogs_->SetTable(table_contact_persons_, wxGrid::wxGridSelectRows, 2500);
  grid_catalogs_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
  grid_catalogs_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_CATALOGS", (wxWindow *)grid_catalogs_);
  Connect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddInCatalogClick);
  Connect(XRCID("ID_CHOICE_CATALOG"), wxEVT_COMMAND_CHOICE_SELECTED,
          (wxObjectEventFunction)&GridFrame::OnCatalogSelect);

  contact_persons_frame_ = new ContactPersonsFrame();
  wxXmlResource::Get()->LoadFrame(contact_persons_frame_, this,
                                  _T("ContactPersonsFrame"));
  contact_persons_frame_->Init();

  Centre();
  GetSizer()->RecalcSizes();
}

void GridFrame::InitContactPersonsTable() {
  std::vector<Column> cols;
  cols.push_back(Column(0, "Имя", 150));
  cols.push_back(Column(1, "Код", 100));
  cols.push_back(Column(2, "Email", 50));
  cols.push_back(Column(4, "Телефон", 100));
  cols.push_back(Column(5, "Факс", 100));
  cols.push_back(Column(3, "Заметки", 200));
  ModelP model = ContactPerson::GetModel(ModelBackendP(
    new ServerModelBackend(Options::client(), "contact_persons")));
  table_contact_persons_ =
    new ContraptionGridTableBase(model->All(), cols);
}

void GridFrame::OnCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
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

void GridFrame::OnAddInCatalogClick(wxCommandEvent& WXUNUSED(event)) {
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

void GridFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
