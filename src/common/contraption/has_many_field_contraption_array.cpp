#include "has_many_field_contraption_array.hpp"
// std
#include <algorithm>
#include <cassert>
// common
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <iostream> // oops

using namespace std;
using namespace tms::common::contraption;

void HasManyFieldContraptionArray::push_back(ContraptionP contraption) {
  if (ContraptionAccessor(&*contraption).id() 
      == 
      Contraption::kNewID) {
    throw(ModelBackendException(
        "Only contraptions already saved to database "
        "can be added to the HasManyContraptionArray"));
  }
  ContraptionArray::push_back(contraption);
  to_add_.push_back(ContraptionP(new Contraption(
      *contraption)));
}


void HasManyFieldContraptionArray::Save() {
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

  for (size_t pos = 0, end = to_add_.size(); pos < end; ++pos) {
    try {
      ContraptionP link = through_model_->New();
      link->Set<int>(id_column_->field_id(), static_cast<int>(id_));
      link->Set<int>(other_id_column_->field_id(), 
                     static_cast<int>(ContraptionAccessor(&*to_add_.at(pos)).id()));
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
  to_add_.clear();
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
    id_(id),
    to_add_() {
  ContraptionArrayP links 
      = through_model_->Filter(Compare(id_column_, 
                                       kEqual, 
                                       static_cast<int>(id_)));
  for (size_t pos = 0, end = links->size(); pos < end; ++pos) {
    ContraptionP contraption(model_->New());
    ContraptionAccessor accessor(&*contraption);
    accessor.id() = static_cast<ContraptionID>(
        links->at(pos)->Get<int>(other_id_column_->field_id()));
    ContraptionArray::push_back(contraption);
  }

  Init();
}

ContraptionArrayP HasManyFieldContraptionArray::Duplicate() {
  return ContraptionArrayP(new HasManyFieldContraptionArray(model_, through_model_, id_column_, other_id_column_, id_));
}
