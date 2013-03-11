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
    display_(display),
    contraption_factory_(boost::bind(
        &ContraptionChoice::DefaultContraptionFactory, this)) {
  wxXmlResource::Get()->Load(_T("xrc/common/widget/pnl_contraption_choice.xrc"));    
  wxASSERT(wxXmlResource::Get()->LoadPanel(this, parent,
                                           _T("pnlContraptionChoice")));
  Init();
}

ContraptionChoice::~ContraptionChoice() {}

ContraptionP ContraptionChoice::DefaultContraptionFactory() {
  return contraptions_->model()->New();
}

void ContraptionChoice::Init() {
  choice_ = XRCCTRL(*this, "chcChoice", wxChoice); 
  XRCCTRL(*this, "btnShow", wxButton)->
      Bind(wxEVT_COMMAND_BUTTON_CLICKED,
           boost::bind(&ContraptionChoice::ShowContraption, this));
  XRCCTRL(*this, "btnAdd", wxButton)->
      Bind(wxEVT_COMMAND_BUTTON_CLICKED,
           boost::bind(&ContraptionChoice::AddContraption, this));
  choice_->Bind(wxEVT_COMMAND_CHOICE_SELECTED, 
                boost::bind(&ContraptionChoice::OnChoiceChange, this, _1));
}

void ContraptionChoice::ShowContraption() {
  ContraptionP contraption = GetSelection();
  if (contraption_dialog_ && contraption) {
    contraption_dialog_->SetUpValues(contraption, contraptions_);
    contraption_dialog_->ShowModal();
    ChooseContraption(contraption);
  }
}

void ContraptionChoice::AddContraption() {
  ContraptionP contraption = contraption_factory_();
  if (contraption_dialog_ && contraption) {
    contraption_dialog_->SetUpValues(contraption, contraptions_);
    contraption_dialog_->ShowModal();
    ChooseContraption(contraption);
  }
  on_choice_(GetSelection());
}

void ContraptionChoice::Clear() {
  choice_->Clear();
}

void ContraptionChoice::BindOnChoice(OnChoice event_sink) {
  on_choice_.connect(event_sink);
}

void ContraptionChoice::ClearOnChoiceBinds() {
  on_choice_.disconnect_all_slots(); 
}

void ContraptionChoice::RefreshList() {
  wxString selection = choice_->GetString(choice_->GetSelection());
  choice_->Clear();
  for (size_t i = 0; i < contraptions_->size(); ++i) {
    choice_->Append(wxString::FromUTF8(display_(contraptions_->at(i)).c_str()));
  }
  choice_->SetSelection(choice_->FindString(selection));
}

void ContraptionChoice::OnChoiceChange(wxCommandEvent &WXUNUSED(event)) {
  on_choice_(GetSelection());
}

bool ContraptionChoice::ChooseContraption(contraption::ContraptionP contraption) {
  RefreshList();
  bool found = false;
  if (contraption) {
    for (size_t i = 0; i < contraptions_->size(); ++i) {
      if (contraptions_->at(i)->Get<int>("id") == contraption->Get<int>("id")) {
        found = true;
        choice_->SetSelection(i);
      }
    }
  } 
  if (!found) {
    choice_->SetSelection(wxNOT_FOUND);
    found = true;
  }
  on_choice_(GetSelection());
  return found;
}

contraption::ContraptionP ContraptionChoice::GetSelection() {
  RefreshList();
  if (choice_->GetSelection() == wxNOT_FOUND) {
    return ContraptionP();
  } else {
    return contraptions_->at(choice_->GetSelection());
  }
}

}
}
}
