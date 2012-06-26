#include <contraption/contraption.hpp>
#include <contraption/field_type.hpp>
#include <contraption/field.hpp>
#include "model.hpp"

using namespace tms::common::contraption;
using namespace std;

size_t Model::GetFieldNumber() const
    throw() {
  return fields_.size();
}

FieldType* Model::GetFieldValue(FieldID field_id,
                                Contraption *contraption) const
    throw(FieldException, ModelBackendException) {
  if (field_id > GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in GetField: "
        << field_id;
    throw FieldException(msg.str());
  }
  return backend_->GetField(field_id, contraption);
}

FieldType* Model::GetFieldValue(
    const std::string &field_name,
    Contraption *contraption) const
    throw(FieldException, ModelBackendException) {
  return GetFieldValue(GetFieldID(field_name), contraption);
}

Field* Model::GetField(FieldID field_id) const
    throw(FieldException) {
  return fields_[field_id].get();
}

FieldID Model::GetFieldID(const std::string &field_name) const
    throw(FieldException) {
  map<string, FieldID>::const_iterator it = 
      fields_by_name_.find(field_name);
  if (it == fields_by_name_.end()) {
    ostringstream msg;
    msg << "Incorrect field_name in GetField: "
        << field_name;
    throw FieldException(msg.str());
  }
  return it->second;
}

void Model::Save(Contraption *contraption) const
    throw(ModelBackendException) {
  backend_->Save(contraption);
}

Model::Model(const std::vector<Field*> &fields, 
             ModelBackend *backend) throw() : 
    fields_(fields.size()),
    fields_by_name_(),
    backend_(backend) {
  for (size_t pos = 0, end = fields.size(); pos < end; ++pos) {
    fields_[pos].reset(fields[pos]);
  }
}
