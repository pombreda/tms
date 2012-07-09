// Headers
//------------------------------------------------------------
//------------------------------------------------------------
#include <sstream>
#include <contraption/field_type.hpp>
#include <contraption/field.hpp>
#include "contraption.hpp"

using namespace std;
using namespace tms::common::contraption;

const ContraptionID Contraption::kNewID = 0;

FieldTypeP Contraption::GetFieldValue(size_t field_id)
    throw(FieldException, ModelBackendException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Contraption::GetFieldValue: '" 
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  if (!IsReadable(field_id)) {
    ostringstream msg;
    msg << "Trying to read non-readable field in Contraption::GetFieldValue: '" 
        << GetName(field_id) << "'.";
    throw FieldException(msg.str());
  }
  return model_->GetFieldValue(field_id, values_, id_);
}

void Contraption::SetFieldValue(FieldID field_id, const FieldType& value)
    throw(FieldException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Contraption::SetFieldValue: '" 
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  if (!IsWritable(field_id)) {
    ostringstream msg;
    msg << "Trying to write non-writable field in Contraption::SetFieldValue: '" 
        << GetName(field_id) << "'.";
    throw FieldException(msg.str());
  }
  model_->SetFieldValue(field_id, value, values_, id_);
  on_change_();    
}

void Contraption::Save()
    throw(ModelBackendException) {
  model_->Save(values_, id_);
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

bool Contraption::IsReadable(FieldID field_id) const
    throw(FieldException) {
  return model_->GetField(field_id)->IsReadable();
}

bool Contraption::IsReadable(const string &field) const
    throw(FieldException) {
  return IsReadable(GetID(field));
}

bool Contraption::IsWritable(FieldID field_id) const
    throw(FieldException) {
  return model_->GetField(field_id)->IsWritable();
}

bool Contraption::IsWritable(const string &field) const
    throw(FieldException) {
  return IsWritable(GetID(field));
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
