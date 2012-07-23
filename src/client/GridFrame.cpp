#include "GridFrame.h"

#include <wx/xrc/xmlres.h>

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
END_EVENT_TABLE()

GridFrame::~GridFrame() {
}

static void init(ModelBackendP &backend, ModelP &model) {
  std::string test_db("test.sqlite3");
  remove(test_db.c_str());
  SOCIDBScheme scheme(soci::sqlite3, test_db);
  backend.reset(new SOCIModelBackend(scheme, "test"));
  std::vector<Field*> fields;
  fields.push_back(new SimpleFieldT<std::string>("name"));
  fields.push_back(new SimpleFieldT<int>("age"));
  fields.push_back(new SimpleFieldT<int>("password",
                                         _is_readable = false));
  fields.push_back(new SimpleFieldT<std::string>("Surname",
                                            _backend_name = "surname"));
  model.reset(new Model(fields, backend));
  model->InitSchema();
  ContraptionP test_contraption;
  for (int i = 0; i < 25; i++) {
    test_contraption = model->New();
    test_contraption->Set<std::string>("name", std::string("John") + boost::lexical_cast<std::string>(i % 100));
    test_contraption->Set<int>("age", i % 40 + 10);
    test_contraption->Set<std::string>("Surname", std::string("Smith") + boost::lexical_cast<std::string>(i % 100));
    test_contraption->Save();
    if ((i + 1) % 5 == 0)
      std::cout << i + 1 << " records are written." << std::endl;
  }
  std::cout << "All records are written!" << std::endl;
}

static void OnCellClick(ContraptionP &contraption, FieldID field_id,
                 ContraptionArrayP contraptions) {
}

void GridFrame::Init() {
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddClick);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnExitClick);
  Centre();
  ModelBackendP backend;
  ModelP model;
  init(backend, model);
  ContraptionArrayP contraptions = model->All();
  std::vector<Column> cols;
  cols.push_back(Column(0, "Name", 70));
  cols.push_back(Column(3, "Surname", 100));
  cols.push_back(Column(1, "Age", 50));
  grid_ = new ContraptionGrid(contraptions, cols, this, wxID_ANY);
  wxXmlResource::Get()->AttachUnknownControl("ID_CUSTOM1", (wxWindow *)grid_);
  grid_->SetOnCellClick(boost::bind(&OnCellClick, _1, _2, contraptions));
  grid_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2, contraptions));
  GetSizer()->RecalcSizes();
}

void GridFrame::OnCellDClick(ContraptionP &contraption, FieldID field_id,
                             ContraptionArrayP contraptions) {
  TestFrame *test_frame = (TestFrame *)wxXmlResource::Get()->LoadFrame(this, _T("TestFrame"));
  test_frame->Init();
  test_frame->SetUpValues(contraption->Get<std::string>("name"),
                          contraption->Get<std::string>("Surname"),
                          contraption->Get<int>("age"));
  test_frame->Show(true);
}

void GridFrame::OnAddClick(wxCommandEvent& WXUNUSED(event)) {
  TestFrame *test_frame = (TestFrame *)wxXmlResource::Get()->LoadFrame(this, _T("TestFrame"));
  test_frame->Init();
  test_frame->SetUpValues("", "", 0);
  test_frame->Show(true);
}

void GridFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}
