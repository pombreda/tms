#include "contraption_array.hpp"
// std
#include <algorithm>
// contraption
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>


using namespace std;
using namespace tms::common::contraption;

ContraptionArray::ContraptionArray(
    const vector<ContraptionP> &contraptions,
    ModelP model)
    throw() :
    vector<ContraptionP>(contraptions),
    model_(model) {
  Init();
}

void ContraptionArray::Init() {
  for (size_t i = 0; i < size(); i++) {
    connections_.push_back(SlotConnectionP(new SlotConnection(
        at(i)->on_change_.connect(boost::bind(&ContraptionArray::OnChange, this)))));
    connections_.push_back(SlotConnectionP(new SlotConnection(
        at(i)->on_delete_.connect(boost::bind(&ContraptionArray::erase, this, i)))));
  }
}

void ContraptionArray::push_back(ContraptionP contraption) {
  if (contraption->model().get() != model_) {
    throw(ModelBackendException(
        "Contraption was created from wrong model in "
        "ConrraptionArray::Save."));              
  }
  vector<ContraptionP>::push_back(ContraptionP(new Contraption(*contraption)));
  contraption->on_change_.connect(
      boost::bind(&ContraptionArray::OnChange, this));
  contraption->on_delete_.connect(
      boost::bind(&ContraptionArray::erase, this, size() - 1));
}

void ContraptionArray::erase(size_t position) {
  to_remove_.push_back(at(position));
  vector<ContraptionP>::erase(begin() + position);
  OnChange();
}

ContraptionP ContraptionArray::at(size_t position) {
  return vector<ContraptionP>::at(position);
}

size_t ContraptionArray::size() {
  return vector<ContraptionP>::size();
}

void ContraptionArray::SetOnChange(boost::function<void ()> f) {
  on_change_.connect(f);
}

void ContraptionArray::OnChange() {
  on_change_();
}
