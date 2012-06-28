#include <contraption/contraption.hpp>
#include <contraption/field_type.hpp>
#include <contraption/field.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/selector.hpp>
#include <contraption/selector/all_selector.hpp>
#include <boost/bind.hpp>
#include "model.hpp"
#include <iostream> //oops

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
  vector<RecordP> out(0);
  fields_[field_id]->GetReadRecords(contraption, out);
  if (!out.empty()) {
    backend_->ReadRecords(out, id);
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

void Model::InitSchema()
    throw(ModelBackendException) {
  ContraptionP contraption(new Contraption(ModelP(this)));
  vector<RecordP> out(0);
  for (FieldID field_id = 0, end = fields_.size();
       field_id < end; ++field_id) {
    fields_[field_id]->GetReadRecords(contraption.get(), out);
  }
}

void Model::Save(Contraption *contraption, ContraptionID &id) const
    throw(ModelBackendException) {
  vector<RecordP> out(0);
  for (FieldID field_id = 0, end = fields_.size();
       field_id < end; ++field_id) {
    fields_[field_id]->GetWriteRecords(contraption, out);
  }
  backend_->WriteRecords(out, id);
}

void Model::Delete(ContraptionID &id) const
    throw(ModelBackendException) {
  backend_->DeleteEntry(id);
}

void Model::SaveHandle(const vector<ContraptionP> &save, 
                       const vector<ContraptionP> &remove)
    throw(ModelBackendException) {
  ostringstream msg;
  for (size_t pos = 0, end = save.size(); pos < end; ++pos) {
    try {
      if (ContraptionAccessor(save[pos].get()).model().get() != this) {
        throw(ModelBackendException(
            "Contraption was created from wrong model in "
            "ConrraptionArray::Save."));              
      }
      save[pos]->Save();
    } catch (const ModelBackendException &e) {
      if (msg.str().size() != 0) {
        msg << "\n==\n";
      }
      msg << e.message();
    }
  }
  for (size_t pos = 0, end = remove.size(); pos < end; ++pos) {
    try {
      save[pos]->Delete();
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
}

ContraptionArrayP Model::All()
    throw(ModelBackendException) {
  auto_ptr< vector<ContraptionID> > ids = backend_->Select(
      boost::scoped_ptr<Selector>(new AllSelector()).get());
  vector<ContraptionP> contraptions;
  for (size_t pos = 0, end = ids->size(); pos < end; ++pos) {
    contraptions.push_back(ContraptionP(new Contraption(this)));
    ContraptionAccessor accessor(contraptions[pos].get());
    accessor.id() = (*ids)[pos];
  }
  auto_ptr<ContraptionArray::SaverType> saver(
      new ContraptionArray::SaverType());
  saver->connect(boost::bind(&Model::SaveHandle, this, _1, _2));
  return ContraptionArrayP(
      new ContraptionArray(saver, contraptions));
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
    fields_(),
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
