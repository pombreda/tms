#include "grid_frame.hpp"

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
  for (int i = 0; i < 5; i++) {
    test_contraption = model->New();
    test_contraption->Set<std::string>("name", std::string("John") +
                                       boost::lexical_cast<std::string>(i % 100));
    test_contraption->Set<int>("age", i % 40 + 10);
    test_contraption->Set<std::string>("Surname", std::string("Smith") +
                                       boost::lexical_cast<std::string>(i % 100));
    test_contraption->Save();
    if ((i + 1) % 5 == 0)
      std::cout << i + 1 << " records are written." << std::endl;
  }
  std::cout << "All records are written!" << std::endl;
}

void GridFrame::Init() {
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddClick, this);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnExitClick, this);
  Centre();
  ModelBackendP backend;
  ModelP model;
  init(backend, model);
  contraptions_ = model->All();
  std::vector<Column> cols;
  cols.push_back(Column(0, "Name", 70));
  cols.push_back(Column(3, "Surname", 100));
  cols.push_back(Column(1, "Age", 50));
  grid_ = new ContraptionGrid(contraptions_, cols, this, wxID_ANY);
  wxXmlResource::Get()->AttachUnknownControl("ID_CUSTOM1", (wxWindow *)grid_);
  grid_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
  grid_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
  GetSizer()->RecalcSizes();
  test_frame = new TestFrame();
  wxXmlResource::Get()->LoadFrame(test_frame, this, _T("TestFrame"));
  test_frame->Init();
}

void GridFrame::OnCellClick(ContraptionP contraption, FieldID field_id) {
}

void GridFrame::OnCellDClick(ContraptionP contraption, FieldID field_id) {
  test_frame->SetUpValues(contraption, contraptions_);
  test_frame->Show(true);
}

void GridFrame::OnAddClick(wxCommandEvent& WXUNUSED(event)) {
  test_frame->SetUpValues(contraptions_->model()->New(), contraptions_);
  test_frame->Show(true);
}

void GridFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}
