#include "GridFrame.h"

//(*InternalHeaders(GridFrame)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(GridFrame)
//*)

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
	//(*EventTable(GridFrame)
	//*)
END_EVENT_TABLE()

GridFrame::GridFrame(wxWindow* parent)
{
	//(*Initialize(GridFrame)
	wxXmlResource::Get()->LoadObject(this,parent,_T("GridFrame"),_T("wxFrame"));
	grid1 = (ContraptionGrid*)FindWindow(XRCID("ID_GRID1"));
	//*)
}

GridFrame::~GridFrame()
{
	//(*Destroy(GridFrame)
	//*)
	delete grid1;
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
  contraption->Set<std::string>("name", std::string("Ivan"));
  contraptions->erase(contraptions->size() - 1);
}

void GridFrame::Init() {
  ModelBackendP backend;
  ModelP model;
  init(backend, model);
  ContraptionArrayP contraptions = model->All();
  std::vector<Column> cols;
  cols.push_back(Column(0, "Name", 70));
  cols.push_back(Column(3, "Surname", 100));
  cols.push_back(Column(1, "Age", 50));
  grid1 = new ContraptionGrid(contraptions, cols, this,
                             wxID_ANY, wxPoint(0, 0), wxSize(400, 300));
  boost::function<void(ContraptionP, FieldID)> f = boost::bind(&OnCellClick, _1, _2, contraptions);
  grid1->SetOnCellClick(f);
  wxBoxSizer *topSizer = (wxBoxSizer*)GetSizer();
  topSizer->Add(grid1, 20, wxEXPAND);
  SetSize(200, 400);
}
