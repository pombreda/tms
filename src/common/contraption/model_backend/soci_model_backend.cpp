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
#include <contraption/filter/all_filter.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <rtti/typeinfo.hpp>
//
#include "soci_model_backend.hpp"


using namespace tms::common::rtti;
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
    session_ << "SELECT " << id_column_ << " FROM " 
             << table_name_ << " WHERE " << id_column_
             << " = :v", soci::use(id), soci::into(id);
    if (!session_.got_data()) {
      id = Contraption::kNewID;
    }
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

string SQLTypeName(const int &/*dummy*/) {
  return "INTEGER";
}

string SQLTypeName(const string &/*dummy*/) {
  return "LONGTEXT";
}

template<typename T>
static bool PrepareCreateColumn(RecordP record_p,
                         map<string, Record*> &done,
                         ostringstream &query) {
  cerr << "z" << query.fail()  << " " << (int) (&query) << endl;
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
    } else {
      done[record->field] = record;
    }
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
    id = Contraption::kNewID;
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

static void WriteSelector(FilterCP filter,
                          ostringstream &query,
                          soci::statement &st) 
    throw(ModelBackendException);

template<class T>
static bool WriteCompareSelector(FilterCP filter_p,
                                 ostringstream &query,
                                 soci::statement &st) 
    throw(ModelBackendException) {
  const CompareFilterT<T> *filter 
      = dynamic_cast<const CompareFilterT<T>*>(filter_p.get());
  if (filter) {
    query << filter->backend_name() << ' ';
    switch (filter->type()) {
      case kEqual:
        query << "==";
        break;
      case kNotEqual:
        query << "!=";
        break;
      case kLesser:
        query << "<";
        break;
      case kNotLesser:
        query << ">=";
        break;
      case kGreater:
        query << ">";
        break;
      case kNotGreater:
        query << "<=";
        break;
      default:
        ostringstream msg;
        msg << "Unsupported CompareFilter type '" << filter->type()
            << "' in SOCIModelBackend::Select.";
        throw ModelBackendException(msg.str());
    }
    query << " :v" << query.str().size();
    st.exchange(soci::use(filter->value()));
    return true;
  }
  return false;
}


static bool WriteNotSelector(FilterCP filter_p,
                             ostringstream &query,
                             soci::statement &st) 
    throw(ModelBackendException) {
  const NotFilter *filter 
      = dynamic_cast<const NotFilter*>(filter_p.get());
  if (filter) {
    query << "NOT (";
    WriteSelector(filter->filter(), query, st);
    query << ")";
    return true;
  }
  return false;
}

static bool WriteAndSelector(FilterCP filter_p,
                             ostringstream &query,
                             soci::statement &st) 
    throw(ModelBackendException) {
  const AndFilter *filter 
      = dynamic_cast<const AndFilter*>(filter_p.get());
  if (filter) {
    query << "(";
    WriteSelector(filter->filter1(), query, st);
    query << ") AND (";
    WriteSelector(filter->filter2(), query, st);
    query << ")";
    return true;
  }
  return false;
}

static bool WriteOrSelector(FilterCP filter_p,
                            ostringstream &query,
                            soci::statement &st) 
    throw(ModelBackendException) {
  const OrFilter *filter 
      = dynamic_cast<const OrFilter*>(filter_p.get());
  if (filter) {
    query << "(";
    WriteSelector(filter->filter1(), query, st);
    query << ") OR (";
    WriteSelector(filter->filter2(), query, st);
    query << ")";
    return true;
  }
  return false;
}

static void WriteSelector(FilterCP filter,
                          ostringstream &query,
                          soci::statement &st) 
    throw(ModelBackendException) {
  if (WriteCompareSelector<int>(filter, query, st)) {
    return;
  }
  if (WriteCompareSelector<string>(filter, query, st)) {
    return;
  }
  if (WriteNotSelector(filter, query, st)) {
    return;
  }
  if (WriteAndSelector(filter, query, st)) {
    return;
  }
  if (WriteOrSelector(filter, query, st)) {
    return;
  }
  ostringstream msg;
  msg << "Unsupported filter '" << TypeID(*filter).name()
      << "' in SOCIModelBackend::Select.";
  throw ModelBackendException(msg.str());
}

auto_ptr< vector<ContraptionID> > SOCIModelBackend::Select(
    FilterCP filter)
    throw(ModelBackendException) {
  try {
    OpenSession();    
    const AllFilter *all_filter 
        = dynamic_cast<const AllFilter*>(filter.get());
    ostringstream query;
    soci::statement st(session_);
    if (all_filter) {
      query << "SELECT " << id_column_ 
            << " FROM " << table_name_;
    } else {
      query << "SELECT " << id_column_ << " FROM " << table_name_ 
            << " WHERE ";
      WriteSelector(filter, query, st);
    }
    ContraptionID id;
    st.exchange(soci::into(id));
    st.alloc();
    st.prepare(query.str());
    st.define_and_bind();
    st.execute();
    auto_ptr< vector<ContraptionID> > ret(new vector<ContraptionID>());
    while(st.fetch()) {
      ret->push_back(id);
    }
    return ret;
  } catch (const ModelBackendException &e) {
    throw;
  } catch (const exception &e) {
    throw ModelBackendException(&e, "Exception in SOCIModelBackend::Select.");
  } catch (...) {
    throw ModelBackendException("Unsupported exception type in "
                                "SOCIModelBackend::Select.");
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
