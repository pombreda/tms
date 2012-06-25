//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include "contraption.hpp"

using namespace std;
using namespace tmd::common::contraption;

Contraption::kNewID = 0;

void Contraption::Save() {
  model_->Save(this);
}

void Contraption::Refresh() {
  FreeValues();
  InitValues();
}

FieldType* Contraption::GetType(FieldID field_id) {
  return model_->GetField(field_id)->type();
}

FieldType* Contraption::GetType(const string &field) {
  return model_->GetField(field)->type();
}

size_t Contraption::GetFieldNumber() {
  return model_->GetFieldNumber();
}

FieldID Contraption::GetFieldID(const string &field) {
  return model_->GetFieldID(field);
}

std::string GetFieldName(FieldID field_id) {
  return model_->GetFieldName(field_id); 
}

void Contraption::InitValues() {
  values_.assign(FieldNumber(), 0);
}

void Contraption::FreeValues() {
  for (size_t field_id = 0, end = GetFieldNumber(); field_id < end; ++field_id) {
    if (other.values_[field_id]) {
      GetType(field_id)->Free(other.values_[field_id]);
    }
  }
}

Contraption::Contraption(const Contraption &other) {
  model_ = other.model;
  id_ = other.id;
  InitValues();
  for (size_t field_id = 0, end = GetFieldNumber(); field_id < end; ++field_id) {
    if (other.values_[field_id]) {
      values_[field_id] = GetType(field_id)->Duplicate(other.values_[field_id]);
    }
  }
}

Contraption::Contraption(const Model* model) {
  model_ = model;
  id_ = kNewID;
  InitValues();
}

Contraption::Contraption(const Model* model) {}

Contraption::~Contraption(){
  FreeValues();
}

