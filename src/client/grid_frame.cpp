#include "grid_frame.hpp"

namespace tms{
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::widget;

BEGIN_EVENT_TABLE(GridFrame,wxFrame)
END_EVENT_TABLE()

GridFrame::~GridFrame() {
}

void GridFrame::Init(ContraptionGridTableBase *contact_persons) {
  Connect(XRCID("ID_BUTTON_ADD_IN_BOOK"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddClick);
  Connect(XRCID("ID_BUTTON_ADD_IN_CATALOG"), wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction)&GridFrame::OnAddClick);
  Centre();
  base_ = contact_persons;
  contraptions_ = base_->contraptions();
  grid_ = new ContraptionGrid(this, wxID_ANY);
  grid_->SetTable(base_, wxGrid::wxGridSelectRows, 2500);
  grid_->SetOnCellClick(boost::bind(&GridFrame::OnCellClick, this, _1, _2));
  grid_->SetOnCellDClick(boost::bind(&GridFrame::OnCellDClick, this, _1, _2));
  wxXmlResource::Get()->AttachUnknownControl("ID_GRID_CATALOGS", (wxWindow *)grid_);
  GetSizer()->RecalcSizes();
  contact_persons_frame = new ContactPersonsFrame();
  wxXmlResource::Get()->LoadFrame(contact_persons_frame, this,
                                  _T("ContactPersonsFrame"));
  contact_persons_frame->Init();
}

void GridFrame::OnCellClick(ContraptionP /*contraption*/, FieldID /*field_id*/) {
}

void GridFrame::OnCellDClick(ContraptionP contraption, FieldID /*field_id*/) {
  contact_persons_frame->SetUpValues(contraption, contraptions_);
  contact_persons_frame->Show(true);
}

void GridFrame::OnAddClick(wxCommandEvent& WXUNUSED(event)) {
  contact_persons_frame->SetUpValues(contraptions_->model()->New(), contraptions_);
  contact_persons_frame->Show(true);
}

void GridFrame::OnExitClick(wxCommandEvent& WXUNUSED(event)) {
  Close();
}

}
}
