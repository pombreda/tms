#include "contraption_choice.hpp"
// wx
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>

namespace tms {
namespace common {
namespace widget {
using namespace contraption;
ContraptionChoice::ContraptionChoice(wxWindow *parent, 
                                     DisplayFunction display) :
    wxPanel(),
    contraptions_(),
    model_(),
    choice_(),
    display_(display) {
  wxXmlResource::Get()->Load(_T("xrc/common/widget/pnl_contraption_choice.xrc"));    
  assert(wxXmlResource::Get()->LoadPanel(this, parent,
                                         _T("pnlContraptionChoice")));
  choice_ = XRCCTRL(*this, "chcChoice", wxChoice); 
}

ContraptionChoice::~ContraptionChoice() {}

void ContraptionChoice::RefreshList() {
  wxString selection = choice_->GetString(choice_->GetSelection());
  choice_->Clear();
  for (size_t i = 0; i < contraptions_->size(); ++i) {
    choice_->Append(wxString::FromUTF8(display_(contraptions_->at(i)).c_str()));
  }
  choice_->SetSelection(choice_->FindString(selection));
}

bool ContraptionChoice::ChooseContraption(contraption::ContraptionP contraption) {
  RefreshList();
  bool found = false;
  for (size_t i = 0; i < contraptions_->size(); ++i) {
    if (contraptions_->at(i)->Get<int>("id") == contraption->Get<int>("id")) {
      found = true;
      choice_->SetSelection(i);
    }
  }
  return found;
}

contraption::ContraptionP ContraptionChoice::GetSelection() {
  RefreshList();
  return contraptions_->at(choice_->GetSelection());
}

}
}
}
