#include <contraption/contraption.hpp>
#include <contraption/field_type.hpp>
#include <contraption/field.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/selector.hpp>
#include <contraption/selector/all_selector.hpp>
#include <boost/mem_fn.hpp>
#include "model.hpp"

using namespace tms::common::contraption;
using namespace std;

size_t Model::GetFieldNumber() const
    throw() {
  return fields_.size();
}

void Model::GetFieldValue(FieldID field_id,
                          Contraption *contraption,
                          ContraptionID id) const
    throw(FieldException, ModelBackendException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Model::GetField: '"
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  Field *field = fields_[field_id].get();
  vector<Record*> out(0);
  const GUIException *ex = 0;
  try {
    field->GetReadRecords(contraption, out);
    backend_->ReadRecords(out, id);
  } catch (const FieldException &e) {
    ex = &e;
  } catch (const ModelBackendException &e) {
    ex = &e;
  }
  for (size_t pos = 0, end = out.size(); pos < end; ++pos) {
    delete out[pos];
  }
  if (ex) {
    throw *ex;
  }
}

void Model::GetFieldValue(
    const std::string &field_name,
    Contraption *contraption,
    ContraptionID id) const
    throw(FieldException, ModelBackendException) {
  GetFieldValue(GetFieldID(field_name), contraption, id);
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

void Model::Save(Contraption *contraption, ContraptionID &id) const
    throw(ModelBackendException) {
  vector<Record*> out(0);
  const GUIException *ex = 0;
  try {
    for (FieldID field_id = 0, end = fields_.size();
         field_id < end; ++field_id) {
      fields_[field_id]->GetWriteRecords(contraption, out);
    }
    backend_->WriteRecords(out, id);
  } catch (const FieldException &e) {
    ex = &e;
  } catch (const ModelBackendException &e) {
    ex = &e;
  }
  for (size_t pos = 0, end = out.size(); pos < end; ++pos) {
    delete out[pos];
  }
  if (ex) {
    throw *ex;
  }
}

void Model::SaveHandle(ContraptionArray& array)
    throw(ModelBackendException) {
  for (size_t pos = 0, end = array.size(); pos < end; ++pos) {
    if (ContraptionAccessor(&array[pos]).model().get() != this) {
      throw(ModelBackendException(
          "Contraption was created from wrong model in ConrraptionArray::Save."));
    }
    array[pos].Save();
  }
}

ContraptionArray Model::GetContraptions() const 
    throw(ModelBackendException) {
  auto_ptr< vector<ContraptionID> > ids = backend_->Select(AllSelector());
  /* vector<Contraption> contraptions;
  for (size_t pos = 0, end = ids->size(); pos < end; ++pos) {
    contraptions.push_back(Contraption(this));
    ContraptionAccessor accessor(contraption).id() = (*ids)[pos];
  }
  std::auto_ptr<boost::signal<void (std::vector<Contraption>&)> > saver(
      new boost::signal<void (std::vector<Contraption>&)>());
  saver->connect(boost::mem_fn(SaveHandle));
  return ContraptionArray(saver, contraptions);*/
}

Model::Model(const vector<Field*> &fields, 
             ModelBackend *backend)
    throw(FieldException) : 
    ptr_count_(0),
    fields_(),
    fields_by_name_(),
    backend_(backend) {
  InitFields(fields);
}

Model::Model(const vector<Field*> &fields, 
             boost::shared_ptr<ModelBackend> backend)
    throw(FieldException) : 
    ptr_count_(0),
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
  for (size_t field_id = 0; field_id < size; ++field_id) {
    fields_[field_id]->Initialize(this);
  }
}
