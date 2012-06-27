// std
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
// common
#include <contraption/record.hpp>
#include <contraption/contraption.hpp> // for kNewID only
//
#include "memory_model_backend.hpp"

using namespace tms::common::contraption;
using namespace std;

void MemoryModelBackend::ReadRecords(
    std::vector<Record*> records,
    ContraptionID id)
    throw(ModelBackendException) {
  try {
    for (size_t pos = 0, end = records.size(); pos < end; ++pos) {
      {
        RecordT<int> *record = dynamic_cast<RecordT<int>*>(records[pos]);
        if (record) {
          *record->data = int_fields_[id][record->field];
          continue;
        }
      }
      {
        RecordT<string> *record = dynamic_cast<RecordT<string>*>(records[pos]);
        if (record) {
          *record->data = string_fields_[id][record->field];
          continue;
        }
      }
      ostringstream msg;
      msg << "Unknown Record type: '" << typeid(*records[pos]).name()
          << "' in MemoryModelBackend::ReadRecords.";
      throw ModelBackendException(msg.str());
    }
  }catch (const ModelBackendException &e) {
    throw e;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception");
  }
}


void MemoryModelBackend::WriteRecords(
    std::vector<Record*> records,
    ContraptionID &id)
    throw(ModelBackendException) {
  if (id == Contraption::kNewID) {
    if (int_fields_.size() > 0) {
      id = int_fields_.rbegin()->first;
    }
    ++id;
    int_fields_[id]["#_id__#"] = (int)id;
  }
  try {
    for (size_t pos = 0, end = records.size(); pos < end; ++pos) {
      {
        RecordT<int> *record = dynamic_cast<RecordT<int>*>(records[pos]);
        if (record) {
          int_fields_[id][record->field] = *record->data;
          continue;
        }
      }
      {
        RecordT<string> *record = dynamic_cast<RecordT<string>*>(records[pos]);
        if (record) {
          string_fields_[id][record->field] = *record->data;
          continue;
        }
      }
      ostringstream msg;
      msg << "Unknown Record type: '" << typeid(*records[pos]).name()
          << "' in MemoryModelBackend::WriteRecords.";
      throw ModelBackendException(msg.str());
    }
  }catch (const ModelBackendException &e) {
    throw e;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception");
  }
}

