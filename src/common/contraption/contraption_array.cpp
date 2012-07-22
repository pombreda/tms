#include <algorithm>
#include <contraption/field_type.hpp>
#include "contraption_array.hpp"

#include <iostream>

using namespace std;
using namespace tms::common::contraption;

void ContraptionArray::Save() {
  for (size_t pos = 0, end = size(); pos < end; ++pos) {
    (*this)[pos]->in_array_ = true;
  }
  size_t rend = 0;
  for (size_t pos = 0, end = back_up_.size(); pos < end; ++pos) {
    if (!back_up_[pos]->in_array_) {
      std::swap(back_up_[pos],back_up_[rend]);
      ++rend;
    }
  }
  back_up_.resize(rend);
  (*saver_)(*this, back_up_);
  for (size_t pos = 0, end = size(); pos < end; ++pos) {
    (*this)[pos]->in_array_ = false;
  }
  back_up_ = *this;
}

ContraptionArray::ContraptionArray(
    auto_ptr<SaverType> saver,
    const vector<ContraptionP> &contraptions,
    ModelP model)
    throw() :
    vector<ContraptionP>(contraptions),
    saver_(saver),
    back_up_(contraptions),
    model_(model) {
  for (size_t i = 0; i < size(); i++) {
    at(i)->on_change_.connect(boost::bind(&ContraptionArray::OnChange, this));
  }
}

void ContraptionArray::push_back(const ContraptionP& contraption) {
  vector::push_back(contraption);
  contraption->on_change_.connect(boost::bind(&ContraptionArray::OnChange, this));
}

void ContraptionArray::erase(size_t position) {
  vector::erase(begin() + position);
}

ContraptionP& ContraptionArray::at(size_t position) {
  return vector::at(position);
}

size_t ContraptionArray::size() {
  return vector::size();
}

void ContraptionArray::SetOnChange(boost::function<void ()> f) {
  on_change_.connect(f);
}

void ContraptionArray::OnChange() {
  on_change_();
}
