#include <algorithm>
#include <contraption/field_type.hpp>
#include "contraption_array.hpp"


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
    const vector<ContraptionP> &contraptions) 
    throw() :
    vector<ContraptionP>(contraptions),
    saver_(saver),
    back_up_(contraptions) {
}

