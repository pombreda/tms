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
    msg << "Incorrect field_id in Contraption::GetFieldValue: '" 
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  if (IsPrivate(field_id)) {
    ostringstream msg;
    msg << "Trying to access private field in Contraption::GetFieldValue: '" 
        << GetName(field_id) << "'.";
    throw FieldException(msg.str());
  }
  model_->GetFieldValue(field_id, this, id_);
  return values_[(int)field_id]->Duplicate();
}

void Contraption::SetFieldValue(FieldID field_id, const FieldType& value)
    throw(FieldException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Contraption::SetFieldValue: '" 
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  if (IsPrivate(field_id)) {
    ostringstream msg;
    msg << "Trying to access private field in Contraption::SetFieldValue: '" 
        << GetName(field_id) << "'.";
    throw FieldException(msg.str());
  }
  if (model_->GetField(field_id)->CheckType(&value)) {
    values_[(int)field_id].reset(value.Duplicate());
    on_change_();
  } else {
    ostringstream msg;
    msg << "Incorrect field type in Contraption::SetFieldValue - field: '"
        << GetName(field_id) << "' of type: '"
        << typeid(model_->GetField(field_id)).name()
        << "' can not accept value of type '"
        << typeid(value).name() << "'.";
    throw FieldException(msg.str());
  }
}

void Contraption::Save()
    throw(ModelBackendException) {
  model_->Save(this, id_);
}

void Contraption::Delete()
    throw(ModelBackendException) {
  model_->Delete(id_);
}

void Contraption::Refresh()
    throw() {
  values_.reset(new boost::scoped_ptr<FieldType>[GetFieldNumber()]);
}

size_t Contraption::GetFieldNumber() const
    throw() {
  return model_->GetFieldNumber();
}

bool Contraption::IsPrivate(FieldID field_id) const
    throw(FieldException) {
  return model_->GetField(field_id)->is_private();
}

bool Contraption::IsPrivate(const string &field) const
    throw(FieldException) {
  return IsPrivate(GetID(field));
}

FieldID Contraption::GetID(const string &field) const
    throw(FieldException) {
  return model_->GetFieldID(field);
}

string Contraption::GetName(FieldID field_id) const
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
    ptr_count_(0),
    in_array_(false),
    on_change_(),
    model_(other.model_),
    values_(new boost::scoped_ptr<FieldType>[GetFieldNumber()]),
    id_(other.id_) {
  for (size_t field_id = 0, end = GetFieldNumber();
       field_id < end; ++field_id) {
    if (other.values_[(int)field_id]) {
      values_[(int)field_id].reset(other.values_[(int)field_id]->Duplicate());
    }
  }
}

Contraption::Contraption(ModelP model) 
    throw():
    ptr_count_(0),
    in_array_(false),
    on_change_(),
    model_(model),
    values_(new boost::scoped_ptr<FieldType>[GetFieldNumber()]),
    id_(kNewID) {
}

Contraption::Contraption(Model* model) 
    throw():
    ptr_count_(0),
    in_array_(false),
    on_change_(),
    model_(model),
    values_(new boost::scoped_ptr<FieldType>[GetFieldNumber()]),
    id_(kNewID) {
}
