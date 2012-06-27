// std
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
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
    const ContraptionID id = accessor.id();
    const Field *field = model->GetField(field_id);
    const SimpleField *simple_field = dynamic_cast<const SimpleField*>(field);
    if (simple_field) {
      const SimpleFieldT<int> *int_field = 
          dynamic_cast<const SimpleFieldT<int>*>(simple_field);
      if (int_field) {
        int value = int_fields_[id][field->name()];
        return new FieldTypeT<int>(value);
      } 
      const SimpleFieldT<string> *string_field = 
          dynamic_cast<const SimpleFieldT<string>*>(simple_field);
      if (string_field) {
        string value = string_fields_[id][field->name()];
        return new FieldTypeT<string>(value);
      } 
    } 
    ostringstream msg;
    msg << "Unknown FieldType: '" << typeid(*field).name()
        << "' for field: '" << contraption->GetFieldName(field_id) 
        << "' in MemoryModelBackend::GetField.";
    throw ModelBackendException(msg.str());
  }catch (const ModelBackendException &e) {
    throw e;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception");
  }
}

void MemoryModelBackend::Save(Contraption *contraption)
    throw(ModelBackendException) {
  ContraptionAccessor accessor(contraption);
  try {
    const Model *model = accessor.model().get();
    ContraptionID &id = accessor.id();
    if (id == Contraption::kNewID) {
      if (int_fields_.size() > 0) {
        id = int_fields_.rbegin()->first;
      }
      ++id;
      int_fields_[id]["#_id__#"] = (int)id;
    }
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
          int_fields_[id][field->name()] = 
              dynamic_cast<const FieldTypeT<int>*>(value)->data();
          continue;
        } 
        const SimpleFieldT<string> *string_field = 
            dynamic_cast<const SimpleFieldT<string>*>(simple_field);
        if (string_field) {
          string_fields_[id][field->name()] = 
              dynamic_cast<const FieldTypeT<string>*>(value)->data();
          continue;
        } 
      }
      ostringstream msg;
      msg << "Unknown FieldType: '" << typeid(*value).name()
          << "' for field: '" << contraption->GetFieldName(field_id) 
          << "' in MemoryModelBackend::Save.";
      throw ModelBackendException(msg.str());
    }
  } catch (const ModelBackendException &e) {
    throw e;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception");
  }
}
