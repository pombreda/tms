#include "has_many_field_contraption_array.hpp"
// std
#include <algorithm>
// common
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <iostream> // oops

using namespace std;
using namespace tms::common::contraption;

void HasManyFieldContraptionArray::Save() {
  ostringstream msg;
  for (size_t pos = 0, end = size(); pos < end; ++pos) {
    try {
      at(pos)->Save();
      ContraptionP link = through_model_->New();
      link->Set<int>(id_column_->field_id(), static_cast<int>(id_));
      link->Set<int>(other_id_column_->field_id(), 
                     static_cast<int>(ContraptionAccessor(&*at(pos)).id()));
      link->Save();
    } catch (const ModelBackendException &e) {
      if (msg.str().size() != 0) {
        msg << "\n==\n";
      }
      msg << e.message();
    }
  }

  for (size_t pos = 0, end = to_remove_.size(); pos < end; ++pos) {
    try {
      ContraptionArrayP links = through_model_->Filter(And(
          Compare(id_column_, kEqual, static_cast<int>(id_)),
          Compare(id_column_, 
                  kEqual, 
                  static_cast<int>(
                      ContraptionAccessor(&*to_remove_.at(pos)).id()))));      
      if (links->size() > 0) {
        links->at(0)->Delete();
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

HasManyFieldContraptionArray::HasManyFieldContraptionArray(
    ModelP model,
    ModelP through_model,
    const IntField *id_column,
    const IntField *other_id_column,
    ContraptionID id)
    throw() :
    ContraptionArray(vector<ContraptionP>(), model),
    through_model_(through_model),
    id_column_(id_column),
    other_id_column_(other_id_column),
    id_(id) {
  Refresh();
}

void HasManyFieldContraptionArray::Refresh() {
  ContraptionArrayP links 
      = through_model_->Filter(Compare(id_column_, 
                                       kEqual, 
                                       static_cast<int>(id_)));
  cerr << links->size() << endl;
  for (size_t pos = 0, end = links->size(); pos < end; ++pos) {
    push_back(ContraptionP(model_->New()));
    ContraptionAccessor accessor(&*at(pos));
    accessor.id() = static_cast<ContraptionID>(
        links->at(pos)->Get<int>(other_id_column_->field_id()));
  }
  Init();
  OnChange();
}
