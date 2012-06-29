//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <map>
#include <sstream>
// boost
#include <boost/foreach.hpp>
// common
#include <contraption/contraption.hpp>
#include <contraption/selector.hpp>
#include <contraption/selector/all_selector.hpp>
//
#include "soci_model_backend.hpp"



using namespace tms::common::contraption;
using namespace std;

// Rewrite using Boost MTL lists?

SOCIModelBackend::SOCIModelBackend(const SOCIDBScheme &scheme, 
                                   const std::string &table_name,
                                   string id_column,
                                   string init_column
                                   ) 
    throw(ModelBackendException) :
    scheme_(scheme),
    table_name_(table_name),
    thread_safe_(false),
    session_(),
    id_column_(id_column),
    init_column_(init_column) {
  if (!thread_safe_) {
    session_.open(scheme_.factory, scheme_.connection_string);
  }
}

void SOCIModelBackend::OpenSession() {
  if (thread_safe_){
    session_.open(scheme_.factory, scheme_.connection_string);
  }
}

void SOCIModelBackend::CloseSession() {
  if (thread_safe_){
    session_.close();
  }
}

template<typename T>
static bool PrepareReadColumn(RecordP record_p,
                       soci::statement &st,
                       ostringstream &query) {
  RecordT<T> *record = dynamic_cast<RecordT<T>*>(record_p.get());
  if (record) {
    query << record->field;
    st.exchange(soci::into(*record->data));
    return true;
  }  
  return false;
} 


void SOCIModelBackend::ReadRecords(
    const std::vector<RecordP> &records,
    ContraptionID id)
    throw(ModelBackendException) {
  if (id == Contraption::kNewID) {
    throw ModelBackendException("Trying to read from "
                                "ContraptionID::kNewID " 
                                "in SOCIModelBackend::ReadRecords.");
  }
  try {
    OpenSession();
    ostringstream query;
    soci::statement st(session_);
    query << "SELECT ";
    bool first = true;
    BOOST_FOREACH(RecordP record, records) {
      if (!first) {
        query << ", ";
      }
      first = false;
      if (PrepareReadColumn<int>(record, st, query)) {
        continue;
      }
      if (PrepareReadColumn<string>(record, st, query)) {
        continue;
      }
      ostringstream msg;
      msg << "Unsupported record type int SOCIModelBackend::ReadRecords. "
          << "Type: '"  << typeid(*record).name() << "'.";
      throw ModelBackendException(msg.str());
    }    
    query <<" FROM " << table_name_ << " WHERE " << id_column_
          << " = :v";
    st.exchange(soci::use(id));
    st.alloc();
    st.prepare(query.str());
    st.define_and_bind();
    st.execute(true);
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::ReadRecords");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::ReadRecords.");
  }
  try {
    CloseSession();
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::ReadRecords");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::ReadRecords.");
  }    
}

template<typename T>
static bool PrepareWriteColumn(RecordP record_p,
                               soci::statement &st,
                               ostringstream &query,
                               int i) {
  RecordT<T> *record = dynamic_cast<RecordT<T>*>(record_p.get());
  if (record) {
    query << record->field << " = :v" << i;
    st.exchange(soci::use(*record->data));
    return true;
  }  
  return false;
} 

template<typename T>
static bool PrepareCreateColumn(RecordP record_p,
                                soci::statement &st,
                                ostringstream &query) {
  RecordT<T> *record = dynamic_cast<RecordT<T>*>(record_p.get());
  if (record) {
    query << record->field << " = :v";
    st.exchange(soci::use(*record->data));
    return true;
  }  
  return false;
} 

void SOCIModelBackend::WriteRecords(
    const std::vector<RecordP> &records,
    ContraptionID &id)
    throw(ModelBackendException) {
  if (records.empty()) {
    return;
  }
  try {
    OpenSession();
    ostringstream query;
    soci::statement st(session_);
    if (id == Contraption::kNewID) {
      session_ << "SELECT COUNT(" << id_column_ << ") FROM " << table_name_,
          soci::into(id);
      if (id > 0) {
        session_ << "SELECT MAX(" << id_column_ << ") FROM " << table_name_,
          soci::into(id);
      }
      ++id;
      if (id == Contraption::kNewID) {
        ++id;
      }
      session_ << "INSERT INTO " << table_name_ << " (" << id_column_ 
               << ") VALUES (" << id << ")";
    }
    query << "UPDATE " << table_name_ << " SET ";
    bool first = true;
    int i = 0;
    BOOST_FOREACH(RecordP record, records) {
      ++i;
      if (!first) {
        query << ", ";
      }
      first = false;
      if (PrepareWriteColumn<int>(record, st, query, i)) {
        continue;
      }
      if (PrepareWriteColumn<string>(record, st, query, i)) {
        continue;
      }
      ostringstream msg;
      msg << "Unsupported record type int SOCIModelBackend::WriteRecords. "
          << "Type: '"  << typeid(*record).name() << "'.";
      throw ModelBackendException(msg.str());
    }    
    query <<" WHERE " << id_column_ << " = :v";
    st.exchange(soci::use(id));
    st.alloc();
    st.prepare(query.str());
    st.define_and_bind();
    st.execute(true);
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::WriteRecords");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::WriteRecords.");
  }
  try {
    CloseSession();
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::WriteRecords");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::WriteRecords.");
  }
}

string SQLTypeName(const int &dummy) {
  return "INTEGER";
}

string SQLTypeName(const string &dummy) {
  return "LONGTEXT";
}

template<typename T>
static bool PrepareCreateColumn(RecordP record_p,
                         map<string, Record*> &done,
                         ostringstream &query) {
  RecordT<T> *record = dynamic_cast<RecordT<T>*>(record_p.get());
  if (record) {
    query << ", " << record->field << " " << SQLTypeName(T());
    map<string, Record*>::iterator it = done.find(record->field);
    if (it != done.end()) {
      RecordT<T> *orecord 
          = dynamic_cast<RecordT<T>*>(it->second);
      if (orecord == 0 || orecord->data != record->data) {
        ostringstream msg;
        msg << "Column name collision in SOCIModelBackend::InitSchema. "
            << "Name: '"  << record->field << "'.";
        throw ModelBackendException(msg.str());
      }
    }
    it->second = record;
    return true;
  }  
  return false;
}

void SOCIModelBackend::InitSchema(
    const vector<RecordP> &records)
    throw(ModelBackendException) {
  try {
    OpenSession();
    map<string, Record*> done;
    ostringstream query;
    session_ << "DROP TABLE IF EXISTS " << table_name_;
    query << "CREATE TABLE " << table_name_ << " ("<< id_column_ 
          << " INTEGER"; 
    BOOST_FOREACH(RecordP record, records) {
      if (PrepareCreateColumn<int>(record, done, query)) {
        continue;
      }
      if (PrepareCreateColumn<string>(record, done, query)) {
        continue;
      }
      ostringstream msg;
      msg << "Unsupported record type int SOCIModelBackend::InitSchema. "
          << "Type: '"  << typeid(*record).name() << "'.";
      throw ModelBackendException(msg.str());
    }
    query << ")";
    session_ << query.str();
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::InitSchema");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::InitSchema.");
  }
  try {
    CloseSession();
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::InitSchema");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::InitSchema.");
  }
}

void SOCIModelBackend::DeleteEntry(
    ContraptionID &id)
    throw(ModelBackendException) {
  if (id == Contraption::kNewID) {
    throw ModelBackendException("Trying to read from "
                                "ContraptionID::kNewID " 
                                "in SOCIModelBackend::DeleteEntry.");
  }
  try {
    OpenSession();
    session_ << "DELETE FROM " << table_name_ << " WHERE "<< id_column_ 
             << " = :id", soci::use(id); 
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::DeleteSchema");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::DeleteRecords.");
  }
  try {
    CloseSession();
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, 
                                "Exception in SOCIModelBackend::DeleteSchema");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::DeleteRecords.");
  }
}

auto_ptr< vector<ContraptionID> > SOCIModelBackend::Select(
    const Selector *selector)
    throw(ModelBackendException) {
  const AllSelector *all_selector 
      = dynamic_cast<const AllSelector*>(selector);
  if (all_selector) {
    soci::rowset<int> rs 
        = (session_.prepare << "SELECT " << id_column_ 
           << " FROM " << table_name_);
    auto_ptr< vector<ContraptionID> > ret(new vector<ContraptionID>());
    for (soci::rowset<int>::const_iterator it = rs.begin(); 
         it != rs.end(); ++it)
    {
      ret->push_back(*it);
    }
    return ret;
  }
  ostringstream msg;
  msg << "Unsupported selector '" << typeid(*selector).name()
      << "' in MemoryModelBackend::Select.";
  throw ModelBackendException(msg.str());
}