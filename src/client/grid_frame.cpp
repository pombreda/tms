#include "grid_frame.hpp"

namespace tms{
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget;

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
END_EVENT_TABLE()

GridFrame::~GridFrame() {
}

void GridFrame::Init(ModelP model, 
                     const std::vector<Column> &cols) {
  Connect(XRCID("ID_BUTTON1"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddClick, this);
  Connect(XRCID("ID_BUTTON2"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnExitClick, this);
  Centre();
  contraptions_ = model->All();
  grid_ = new ContraptionGrid(contraptions_, cols, this, wxID_ANY);
  wxXmlResource::Get()->AttachUnknownControl("ID_CUSTOM1", (wxWindow *)grid_);
  grid_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
  grid_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
  GetSizer()->RecalcSizes();
  test_frame = new TestFrame();
  wxXmlResource::Get()->LoadFrame(test_frame, this, _T("TestFrame"));
  test_frame->Init();
}

void GridFrame::OnCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
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

}
}
