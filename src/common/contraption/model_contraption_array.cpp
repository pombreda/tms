#include "model_contraption_array.hpp"
// std
#include <algorithm>
// common
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>

#include <iostream> // oops

using namespace std;
using namespace tms::common::contraption;

void ModelContraptionArray::Save() {
  ostringstream msg;

  for (size_t pos = 0, end = size(); pos < end; ++pos) {
    try {
      at(pos)->Save();
    } catch (const ModelBackendException &e) {
      if (msg.str().size() != 0) {
        msg << "\n==\n";
      }
      msg << e.message();
    }
  }

  for (size_t pos = 0, end = to_remove_.size(); pos < end; ++pos) {
    try {
      if (ContraptionAccessor(&*to_remove_.at(pos)).id() != Contraption::kNewID) {
        to_remove_.at(pos)->Delete();
      }
    } catch (const ModelBackendException &e) {
      if (msg.str().size() != 0) {
        msg << "\n==\n";
      }
      msg << e.message();
    }
  }

  if (msg.str().size() != 0) {
    string emsg = "Several errors occured:" + msg.str();
    throw(ModelBackendException(emsg));
  }
  to_remove_.clear();
}

ModelContraptionArray::ModelContraptionArray(
    const vector<ContraptionP> &contraptions,
    ModelP model)
    throw() :
    ContraptionArray(contraptions, model) {
}

void ModelContraptionArray::Refresh() {
  swap(*(model_->All()));
  Init();
  OnChange();
}
