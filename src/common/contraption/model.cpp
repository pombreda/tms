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
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Model::GetField: '"
        << field_id << "'.";
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

const Field* Model::GetField(FieldID field_id) const
    throw(FieldException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Model::GetField: '"
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  return fields_[field_id].get();
}

FieldID Model::GetFieldID(const std::string &field_name) const
    throw(FieldException) {
  map<string, FieldID>::const_iterator it = 
      fields_by_name_.find(field_name);
  if (it == fields_by_name_.end()) {
    ostringstream msg;
    msg << "Incorrect field_name in Model::GetField: '"
        << field_name << ".'";
    throw FieldException(msg.str());
  }
  return it->second;
}

void Model::Save(Contraption *contraption) const
    throw(ModelBackendException) {
  backend_->Save(contraption);
}

Model::Model(const vector<Field*> &fields, 
             ModelBackend *backend)
    throw(FieldException) : 
    fields_(),
    fields_by_name_(),
    backend_(backend) {
  InitFields(fields);
}

Model::Model(const vector<Field*> &fields, 
             boost::shared_ptr<ModelBackend> backend)
    throw(FieldException) : 
    fields_(fields.size()),
    fields_by_name_(),
    backend_(backend) {
  InitFields(fields);
}

void Model::InitFields(const std::vector< Field* > &fields)
    throw(FieldException) {
  size_t size = fields.size();
  for (size_t field_id = 0; field_id < size; ++field_id) {
    Field *field = fields[field_id];
    fields_.push_back(boost::shared_ptr<Field>(field));
    if (fields_by_name_.count(field->name()) > 0) {
      ostringstream msg;
      msg << "Duplicate field name in Model::InitFields: '"
          << field->name() << "'";
      throw FieldException(msg.str());    
    }
    fields_by_name_[field->name()] = field_id;
  }
}
