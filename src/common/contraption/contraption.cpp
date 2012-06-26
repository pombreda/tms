//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include "contraption.hpp"
#include <sstream>

using namespace std;
using namespace tmd::common::contraption;

Contraption::kNewID = 0;

FieldType* Contraption::GetField(size_t field_id) const
      throw(FieldExcepton, ModelBackendException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in GetField: " << field_id << ".";
    throw FieldExcepton(msg.str());
  }
  if (!values_[field_id]) {
    values_[field_id].reset(model_->GetField(field_id, this));
  }
  return values_[field_id].get()->Duplicate();
}

FieldType* Contraption::GetField(const std::string &field) const
    throw(FieldExcepton, ModelBackendException) {
  return GetField(GetFieldID(field), this);
}

void Contraption::SetField(size_t field_id, const FieldType *value)
    throw(FieldExcepton) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in SetField: " << field_id << ".";
    throw FieldExcepton(msg.str());
  }
  return values_[field_id].reset(value->Duplicate());
}

void SetField(const std::string &field, const FieldType *value)
    throw(FieldExcepton) {
  SetField(GetFieldID(field), value);
}

void Contraption::Save() const
    throw(ModelBackendException) {
  model_->Save(this, id_);
}

void Contraption::Refresh()
    throw(ModelBackendException) {
  FreeValues();
  InitValues();
}

size_t Contraption::GetFieldNumber() const
    throw() {
  return model_->GetFieldNumber();
}

FieldID Contraption::GetFieldID(const string &field) const
    throw() {
  return model_->GetFieldID(field);
}

std::string GetFieldName(FieldID field_id) const
    throw(FieldExcepton) {
  return model_->GetFieldName(field_id);
}

void Contraption::InitValues() 
    throw() {
  values_.assign(boost::shared_array<FieldType>(), GetFieldNumber());
}

void Contraption::Swap(Contraption &other)
    throw(FieldExcepton) {
  swap(values_, other->values_);
  swap(model_, other->model_);
  swap(id_, model_->id);
}

Contraption& Contraption::operator=(const Contraption &other)
    throw(FieldExcepton) {
  Contraption(other).Swap(*this);
}

Contraption::Contraption(const Contraption &other) {
  model_ = other.model;
  id_ = other.id;
  InitValues();
  for (size_t field_id = 0, end = GetFieldNumber();
       field_id < end; ++field_id) {
    if (other.values_[field_id]) {
      values_[field_id].reset(GetType(field_id)->Duplicate(other.values_[field_id]));
    }
  }
}

Contraption::Contraption(shared_ptr<const Model> model) 
{
  model_ = model;
  id_ = kNewID;
  InitValues();
}

