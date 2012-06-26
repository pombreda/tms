#include "model.hpp"

using namespace tms::common::contraption;
using namespace std;

size_t Model::GetFieldNumber() const 
    throw() {
  return fields_.size();
}

Field* Model::GetField(FieldID field_id, Contraption* contraption) const
    throw(FieldExcepton, ModelBackendException) {
  if (id > GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in GetField: "
        << field_id;
    throw FieldExcepton(msg.str());
  }
  return backend_->GetField(field_id, contraption);
}

Field* Model::GetField(const std::string &field_name) const
    throw(FieldExcepton, ModelBackendException) {
  return GetField(GetFieldID(field));
}

FieldID Model::GetFieldID(const std::string &field_name) const
    throw(FieldExcepton) {
  std::map::const_iterator it = fields_by_name_.find(field);
  if (it == fields_by_name_.end()) {
    ostringstream msg;
    msg << "Incorrect field_name in GetField: "
        << field_name;
    throw FieldExcepton(msg.str());
  }
  return *it;
}

void Save(Contraption *contraption) const
    throw(ModelBackendException) {
  backend_->Save(contraption)
}

Model::Model(const std::vector<Field*> &fields, 
      const ModelBackend *backend) : 
    fields_(fields.size()),
    backend_(backend)
    throw() {
  for (int pos = 0, end = fields.size(); pos < end; ++pos) {
    fields_[pos].reset(fields[pos]);
  }
}
