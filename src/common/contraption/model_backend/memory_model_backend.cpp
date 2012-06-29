// std
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
// common
#include <contraption/record.hpp>
#include <contraption/contraption.hpp> // for kNewID only
#include <contraption/selector.hpp>
#include <contraption/selector/all_selector.hpp>
//
#include "memory_model_backend.hpp"
#include <iostream> //oops

using namespace tms::common::contraption;
using namespace std;

void MemoryModelBackend::ReadRecords(
    const std::vector<RecordP> &records,
    ContraptionID id)
    throw(ModelBackendException) {
  if (id == Contraption::kNewID) {
    throw ModelBackendException("Trying to read from "
                                "ContraptionID::kNewID " 
                                "in MemoryModelBackend::ReadRecords.");
  }
  try {    
    for (size_t pos = 0, end = records.size(); pos < end; ++pos) {
      {
        RecordT<int> *record 
            = dynamic_cast<RecordT<int>*>(records[pos].get());
        if (record) {
          *record->data = int_fields_[id][record->field];
          continue;
        }
      }
      {
        RecordT<string> *record 
            = dynamic_cast<RecordT<string>*>(records[pos].get());
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
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception.");
  }
}


void MemoryModelBackend::WriteRecords(
    const std::vector<RecordP> &records,
    ContraptionID &id)
    throw(ModelBackendException) {
  if (id == Contraption::kNewID) {
    if (!int_fields_.empty()) {
      id = int_fields_.rbegin()->first;
    }
    ++id;
    int_fields_[id]["#_id__#"] = (int)id;
  }
  try {
    for (size_t pos = 0, end = records.size(); pos < end; ++pos) {
      {
        RecordT<int> *record 
            = dynamic_cast<RecordT<int>*>(records[pos].get());
        if (record) {
          int_fields_[id][record->field] = *record->data;
          continue;
        }
      }
      {
        RecordT<string> *record 
            = dynamic_cast<RecordT<string>*>(records[pos].get());
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
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception.");
  }
}

void MemoryModelBackend::DeleteEntry(
    ContraptionID &id)
    throw (ModelBackendException) {
  if (id == Contraption::kNewID) {
    throw ModelBackendException("Trying to delete ContraptionID::kNewID " 
                                "in MemoryModelBackend::DeleteEntry.");
  }
  
  try {
    if (int_fields_.count(id) == 0) {
      ostringstream msg;
      msg << "Trying to delete contraption with id: '" << id
          << "' which does not exist.";
      throw ModelBackendException(msg.str());
    }
    int_fields_.erase(id);
    string_fields_.erase(id);
    id = Contraption::kNewID;
  }catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e);
  } catch (...) {
    throw ModelBackendException("Nonstandart exception.");
  }
}

auto_ptr< vector<ContraptionID> > MemoryModelBackend::Select(
    const Selector *selector)
    throw(ModelBackendException) {
  const AllSelector *all_selector 
      = dynamic_cast<const AllSelector*>(selector);
  if (all_selector) {
    auto_ptr< vector<ContraptionID> > ret(new vector<ContraptionID>());
    for (map<ContraptionID, map<string, int> >::iterator it 
             = int_fields_.begin(), end = int_fields_.end(); 
         it != end; ++it) {
      ret->push_back(it->first);
    }
    return ret;
  }
  ostringstream msg;
  msg << "Unsupported selector '" << typeid(*selector).name()
      << "' in MemoryModelBackend::Select.";
  throw ModelBackendException(msg.str());
}

