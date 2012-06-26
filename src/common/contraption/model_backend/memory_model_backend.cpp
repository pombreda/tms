// std
#include <string>
#include <sstream>
#include <exception>
// common
#include <contraption/field.hpp>
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
//
#include "memory_model_backend.hpp"

using namespace tms::common::contraption;
using namespace std;

FieldType* MemoryModelBackend::GetField(FieldID field_id, 
                                        Contraption *contraption)
    throw(ModelBackendException) {
  try {
    ContraptionAccessor accessor(contraption);
    const Model *model = accessor.model().get();
    const Field *field = model->GetField(field_id);
    const SimpleField *simple_field = dynamic_cast<const SimpleField*>(field);
    if (simple_field) {
      const SimpleFieldT<int> *int_field = 
          dynamic_cast<const SimpleFieldT<int>*>(simple_field);
      if (int_field) {
        int value = int_fields_[field->name()];
        return new FieldTypeT<int>(value);
      } 
      const SimpleFieldT<string> *string_field = 
          dynamic_cast<const SimpleFieldT<string>*>(simple_field);
      if (string_field) {
        string value = string_fields_[field->name()];
        return new FieldTypeT<string>(value);
      } 
    } 
    ostringstream msg;
    msg << "Incompatible FieldType: " << typeid(FieldType).name();
    throw new ModelBackendException(msg.str());
  } catch (const exception &e) {
    throw new ModelBackendException(&e);
  }
}

void MemoryModelBackend::Save(Contraption *contraption)
    throw(ModelBackendException) {
  ContraptionAccessor accessor(contraption);
  try {
    const Model *model = accessor.model().get();
    for (size_t field_id = 0, end = model->GetFieldNumber(); 
         field_id < end; ++field_id) {
      const Field *field = model->GetField(field_id);
      const FieldType *value = accessor.values()[field_id].get();
      if (!value) {
        continue;
      }
      const SimpleField *simple_field = dynamic_cast<const SimpleField*>(field);
      if (simple_field) {
        const SimpleFieldT<int> *int_field = 
            dynamic_cast<const SimpleFieldT<int>*>(simple_field);
        if (int_field) {
          int_fields_[field->name()] = 
              dynamic_cast<const FieldTypeT<int>*>(value)->data();
        } 
        const SimpleFieldT<string> *string_field = 
            dynamic_cast<const SimpleFieldT<string>*>(simple_field);
        if (string_field) {
          string_fields_[field->name()] = 
              dynamic_cast<const FieldTypeT<string>*>(value)->data();
        } 
      }
      ostringstream msg;
      msg << "Incompatible FieldType: " << typeid(FieldType).name();
      throw new ModelBackendException(msg.str());
    }
  } catch (const exception &e) {
    throw new ModelBackendException(&e);
  }
}
