//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <sstream>
#include <contraption/field_type.hpp>
#include <contraption/field.hpp>
#include "contraption.hpp"

using namespace std;
using namespace tms::common::contraption;

const ContraptionID Contraption::kNewID = 0;

FieldType* Contraption::GetFieldValue(size_t field_id)
      throw(FieldException, ModelBackendException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in GetField: " << field_id << ".";
    throw FieldException(msg.str());
  }
  if (!values_[field_id]) {
    values_[field_id].reset(model_->GetFieldValue(field_id, this));
  }
  return values_[field_id].get()->Duplicate();
}

FieldType* Contraption::GetFieldValue(const std::string &field)
    throw(FieldException, ModelBackendException) {
  return GetFieldValue(GetFieldID(field));
}

void Contraption::SetFieldValue(size_t field_id, const FieldType *value)
    throw(FieldException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in SetField: " << field_id << ".";
    throw FieldException(msg.str());
  }
  return values_[field_id].reset(value->Duplicate());
}

void Contraption::SetFieldValue(const std::string &field, const FieldType *value)
    throw(FieldException) {
  SetFieldValue(GetFieldID(field), value);
}

void Contraption::Save()
    throw(ModelBackendException) {
  model_->Save(this);
}

void Contraption::Refresh()
    throw(ModelBackendException) {
  vector< boost::shared_ptr<FieldType> >(GetFieldNumber(), 
                                         boost::shared_ptr<FieldType>()).swap(values_);
}

size_t Contraption::GetFieldNumber() const
    throw() {
  return model_->GetFieldNumber();
}

FieldID Contraption::GetFieldID(const string &field) const
    throw() {
  return model_->GetFieldID(field);
}

string Contraption::GetFieldName(FieldID field_id) const
    throw(FieldException) {
  return model_->GetField(field_id)->name();
}

void Contraption::Swap(Contraption &other)
    throw() {
  swap(values_, other.values_);
  swap(model_, other.model_);
  swap(id_, other.id_);
}

Contraption& Contraption::operator=(const Contraption &other)
    throw() {
  Contraption(other).Swap(*this);
  return *this;
}

Contraption::Contraption(const Contraption &other) 
    throw() :
    model_(other.model_),
    values_(GetFieldNumber(), boost::shared_ptr<FieldType>()),
    id_(other.id_) {
  for (size_t field_id = 0, end = GetFieldNumber();
       field_id < end; ++field_id) {
    if (other.values_[field_id]) {
      values_[field_id].reset(other.values_[field_id]->Duplicate());
    }
  }
}

Contraption::Contraption(boost::shared_ptr<const Model> model) 
    throw():
    model_(model),
    values_(GetFieldNumber(), boost::shared_ptr<FieldType>()),
    id_(kNewID) {
}

