#include "model.hpp"
// boost
#include <boost/bind.hpp>
// common
#include <contraption/contraption.hpp>
#include <contraption/field_type.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/model_contraption_array.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/filter/all_filter.hpp>
// std
#include <algorithm>

using namespace tms::common::contraption;
using namespace std;


class CompareRecordsGr {
 public:
  bool operator()(RecordP a, RecordP b) {
    return a->field > b->field;
  }
};

class CompareRecordsEq {
 public:
  bool operator()(RecordP a, RecordP b) {
    return a->field == b->field;
  }
};


size_t Model::GetFieldNumber() const
    throw() {
  return fields_.size();
}

FieldTypeP Model::GetFieldValue(FieldID field_id,
                                FieldTypeArray &values,
                                ContraptionID id) const
    throw(FieldException, ModelBackendException) {
  if (field_id >= GetFieldNumber()) {
    ostringstream msg;
    msg << "Incorrect field_id in Model::GetField: '"
        << field_id << "'.";
    throw FieldException(msg.str());
  }
  vector<RecordP> out(0);
  fields_[field_id]->GetReadRecords(values, id, out);
  if (!out.empty()) {
    sort(out.begin(), out.end(), CompareRecordsGr());
    out.resize(unique(out.begin(), out.end(), CompareRecordsEq()) - out.begin());
    backend_->ReadRecords(out, id);
  }
  fields_[field_id]->FinalizeGet(values, id);
  return fields_[field_id]->GetValue(values, id);
}


void Model::SetFieldValue(FieldID field_id,
                          const FieldType &value,
                          FieldTypeArray &values,
                          ContraptionID id) const
    throw(FieldException) {
  if (fields_[field_id]->CheckType(&value)) {
    fields_[field_id]->SetValue(value, values, id);
  } else {
    ostringstream msg;
    msg << "Incorrect field type in Contraption::SetFieldValue - field: '"
        << fields_[field_id]->name() << "' of type: '"
        << typeid(fields_[field_id]).name()
        << "' can not accept value of type '"
        << typeid(value).name() << "'.";
    throw FieldException(msg.str());
  }
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

const Field* Model::GetField(const std::string &field_name) const
    throw(FieldException) {
  return GetField(GetFieldID(field_name));
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
  FieldTypeArray values(new boost::scoped_ptr<FieldType>[GetFieldNumber()]);
  vector<RecordP> out(0);
  for (FieldID field_id = 0, end = fields_.size() - 1;
       field_id < end; ++field_id) {
    fields_[field_id]->GetWriteRecords(values, Contraption::kNewID, out);
  }
  sort(out.begin(), out.end(), CompareRecordsGr());
  out.resize(unique(out.begin(), out.end(), CompareRecordsEq()) - out.begin());
  backend_->InitSchema(out);
}

void Model::Save(FieldTypeArray &values,
                 ContraptionID &id) const
    throw(ModelBackendException) {
  vector<RecordP> out(0);

  for (FieldID field_id = 0, end = fields_.size() - 1;
       field_id < end; ++field_id) {
    fields_[field_id]->GetWriteRecords(values, id, out);
  }
  sort(out.begin(), out.end(), CompareRecordsGr());
  out.resize(unique(out.begin(), out.end(), CompareRecordsEq()) - out.begin());
  backend_->WriteRecords(out, id);
  for (FieldID field_id = 0, end = fields_.size() - 1;
       field_id < end; ++field_id) {
    fields_[field_id]->FinalizeSave(values, id);
  }
}

ContraptionP Model::New()
    throw() {
  CheckInit();
  return ContraptionP(new Contraption(ModelP(this)));
}

void Model::Delete(ContraptionID &id) const
    throw(ModelBackendException) {
  backend_->DeleteEntry(id);
}

ContraptionArrayP Model::Filter(FilterCP filter)
    throw(ModelBackendException) {
  auto_ptr< vector<ContraptionID> > ids = backend_->Select(filter);
  vector<ContraptionP> contraptions;
  for (size_t pos = 0, end = ids->size(); pos < end; ++pos) {
    contraptions.push_back(ContraptionP(new Contraption(this)));
    ContraptionAccessor accessor(contraptions[pos].get());
    accessor.id() = (*ids)[pos];
  }
  return ContraptionArrayP(
      new ModelContraptionArray(contraptions, ModelP(this)));
}


ContraptionArrayP Model::All()
    throw(ModelBackendException) {
  return Filter(tms::common::contraption::All());
}

ContraptionArrayP Model::Empty()
    throw(ModelBackendException) {
  return ContraptionArrayP(
      new ModelContraptionArray(vector<ContraptionP>(0), ModelP(this)));
}

Model::Model(const vector<Field*> &fields,
             ModelBackend *backend)
    throw(FieldException) :
    fields_(),
    fields_by_name_(),
    backend_(backend),
    initialized_(false),
    ptr_count_(0) {
  InitFields(fields);
}

Model::Model(const vector<Field*> &fields,
             boost::shared_ptr<ModelBackend> backend)
    throw(FieldException) :
    fields_(),
    fields_by_name_(),
    backend_(backend),
    initialized_(false),
    ptr_count_(0) {
  InitFields(fields);
}

Model::Model(ModelBackend *backend) :
    fields_(),
    fields_by_name_(),
    backend_(backend),
    initialized_(false),
    ptr_count_(0) {
}

Model::Model(boost::shared_ptr<ModelBackend> backend) :
    fields_(),
    fields_by_name_(),
    backend_(backend),
    initialized_(false),
    ptr_count_(0) {  
}

void Model::CheckInit() const
    throw (FieldException) {
  if (!initialized_) {
    throw FieldException("Model was not initialized");
  }
}

bool Model::IsInitialized() const
    throw () {
  return initialized_;
}

void Model::InitFields(const std::vector< Field* > &fields)
    throw(FieldException) {
  if (initialized_) {
    throw FieldException("Model was already initialized");
  }
  size_t size = fields.size();
  for (size_t field_id = 0; field_id < size; ++field_id) {
    Field *field = fields[field_id];
    fields_.push_back(boost::shared_ptr<Field>(field));
    if (fields_by_name_.count(field->name()) > 0) {
      for (size_t i = field_id + 1, end = size; i < end; ++i) {
        delete fields[i];
      }
      ostringstream msg;
      msg << "Duplicate field name in Model::InitFields: '"
          << field->name() << "'";
      throw FieldException(msg.str());
    }
    fields_by_name_[field->name()] = fields_.size() - 1;
  }
  Field *field = new IntField("id", _backend_name = "tms__id_");
  fields_.push_back(boost::shared_ptr<Field>(field));
  fields_by_name_["id"] = fields_.size() - 1;
  for (size_t field_id = 0; field_id < fields_.size(); ++field_id) {
    fields_[field_id]->Initialize(this);
  }
  initialized_ = true;
}

Model::~Model() {
}
