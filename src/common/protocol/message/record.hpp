#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__RECORD_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__RECORD_HPP_
// std
#include <sstream>
// boost
#include <boost/shared_ptr.hpp>
// common
#include <protocol/message/record.pb.h>
#include <contraption/record.hpp>
#include <contraption/model_backend_exception.hpp>
#include <contraption/field_type.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {

typedef boost::shared_ptr<Record> RecordP;

void GetType(const contraption::Record &record, Record &out) 
    throw(contraption::ModelBackendException) {
  using contraption::RecordT;
  if (dynamic_cast<const RecordT<int>*>(&record)) {
    out.set_type(Record::kInt);
    return;
  } 
  if (dynamic_cast<const RecordT<std::string>*>(&record)) {
    out.set_type(Record::kString);
    return;
  } 
  std::ostringstream msg;
  msg << "Unsupported record type int GetType. "
      << "Type: '"  << typeid(record).name() << "'.";
  throw contraption::ModelBackendException(msg.str());
}

void GetName(const contraption::Record &record, Record &out) 
    throw(contraption::ModelBackendException) {
  using contraption::RecordT;
  const RecordT<int>* irecord;
  const RecordT<std::string>* srecord;
  if ((irecord = dynamic_cast<const RecordT<int>*>(&record))) {
    out.set_field(irecord->field);
    return;
  } 
  if ((srecord = dynamic_cast<const RecordT<std::string>*>(&record))) {
    out.set_field(srecord->field);
    return;
  } 
  std::ostringstream msg;
  msg << "Unsupported record type in GetName. "
      << "Type: '"  << typeid(record).name() << "'.";
  throw contraption::ModelBackendException(msg.str());
}

void GetValue(const contraption::Record &record, Record &out)
    throw(contraption::ModelBackendException) {
  using contraption::RecordT;
  const RecordT<int>* irecord;
  const RecordT<std::string>* srecord;
  if ((irecord = dynamic_cast<const RecordT<int>*>(&record))) {
    out.set_int_value(static_cast<google::protobuf::uint32>(*irecord->data));
    return;
  } 
  if ((srecord = dynamic_cast<const RecordT<std::string>*>(&record))) {
    out.set_string_value(*srecord->data);
    return;
  } 
  std::ostringstream msg;
  msg << "Unsupported record type in GetName. "
      << "Type: '"  << typeid(record).name() << "'.";
  throw contraption::ModelBackendException(msg.str());
}

void GetValue(const Record &record, contraption::Record &out)
    throw(contraption::ModelBackendException) {
  using contraption::RecordT;
  RecordT<int>* irecord;
  RecordT<std::string>* srecord;
  if ((irecord = dynamic_cast<RecordT<int>*>(&out))) {
    *irecord->data = static_cast<int>(record.int_value());
    return;
  } 
  if ((srecord = dynamic_cast<RecordT<std::string>*>(&out))) {
    *srecord->data = record.string_value();
    return;
  } 
  std::ostringstream msg;
  msg << "Unsupported record type in GetValue. "
      << "Type: '"  << typeid(record).name() << "'.";
  throw contraption::ModelBackendException(msg.str());
}

void InitRecord(const Record &record, 
                boost::scoped_ptr<contraption::FieldType> &data,
                contraption::RecordP &out) 
    throw(contraption::ModelBackendException) {
  using contraption::RecordT;
  using contraption::FieldTypeT;
  if (record.type() == Record::kInt) {
    FieldTypeT<int> *idata = new FieldTypeT<int>();
    data.reset(idata);
    out.reset(new RecordT<int>(record.field(), &idata->data()));
    if (record.has_int_value()) {
      idata->set_data(static_cast<int>(record.int_value()));
    }
    return;
  } 
  if (record.type() == Record::kString) {
    FieldTypeT<std::string> *sdata = new FieldTypeT<std::string>();
    data.reset(sdata);
    out.reset(new RecordT<std::string>(record.field(), &sdata->data()));
    if (record.has_string_value()) {
      sdata->set_data(record.string_value());
    }
    return;
  } 
  std::ostringstream msg;
  msg << "Unsupported record type in InitValue. "
      << "Type: '"  << record.type() << "'.";
  throw contraption::ModelBackendException(msg.str());
  
}

void GetFreeRecord(const contraption::Record &record, Record &out)
    throw(contraption::ModelBackendException) {
  GetType(record, out);
  GetName(record, out);
}

}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__RECORD_HPP_
