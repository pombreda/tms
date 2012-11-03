#ifndef DATE_FIELD_HPP_INCLUDED
#define DATE_FIELD_HPP_INCLUDED
#include <boost/date_time/posix_time/posix_time.hpp>
namespace pt = boost::posix_time;
#ifndef _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <config.hpp>
// std
#include <typeinfo>
// boost
#include <boost/parameter.hpp>
// common
#include <contraption/field.hpp>
#include <contraption/record.hpp>
#include <contraption/contraption.hpp>
#include <contraption/filter.hpp>
#include <boost/posix_time/posix_time.hpp>

namespace tms {
namespace common {
namespace contraption {

namespace pt = boost::posix_time;

BOOST_PARAMETER_NAME(backend_name)
BOOST_PARAMETER_NAME(default_value)

//------------------------------------------------------------
// SimpleFieldT class. Represents a simple mapping to db.
//------------------------------------------------------------
class DateFieldImpl : public FieldT<pt::ptime> {
 public:
  using FieldT<pt::ptime>::CheckType;
  template <class ArgPack>
  SimpleFieldTImpl(ArgPack const &args)
      throw(FieldException) :
      FieldT<pt::ptime>(args[_name],
                        args[_is_readable | true],
                        args[_is_writable | true]),
      default_value_(args[_default_value | T()]),
      backend_name_(args[_backend_name | args[_name]]) {
  }

  virtual void GetReadRecords(FieldTypeArray &values,
                              ContraptionID id,
                              std::vector<RecordP> &out) const {
    if (!values[static_cast<int>(this->field_id_)]) {
      values[static_cast<int>(this->field_id_)].reset(
          new FieldTypeT<std::string>(default_value_));
      if (id != Contraption::kNewID) {
        out.push_back(RecordP(
            new RecordT<std::string>(backend_name_, &(dynamic_cast<FieldTypeT<std::string>*>(
                values[static_cast<int>(this->field_id_)].get())->data_))));
      }
    }
  }

  virtual void GetWriteRecords(FieldTypeArray &values,
                               ContraptionID id,
                               std::vector<RecordP> &out) const {
    if (!values[static_cast<int>(this->field_id_)]
        && id == Contraption::kNewID) {
      values[static_cast<int>(this->field_id_)].reset(
          new FieldTypeT<std::string>(default_value_));
    }
    if (values[static_cast<int>(this->field_id_)]) {
      out.push_back(RecordP(
          new RecordT<std::string>(backend_name_, &(dynamic_cast<FieldTypeT<std::srting>*>(
              values[static_cast<int>(this->field_id_)].get())->data_))));
    }
  }

  const std::string& backend_name() const
      throw () {
    return backend_name_;
  }

  virtual ~SimpleFieldTImpl() {}

  virtual FieldTypeP GetValue(FieldTypeArray &values, ContraptionID /*id*/) {
    FieldTypeT<std::string> &str_time = dynamic_cast<FieldTypeT<std::sting>&>(
                                          &*values[static_cast<int>(this->field_id_)]);
    return FieldTypeP(new FieldTypeT<pt::ptime>(pt::from_iso_string(str_time.data())));
  }

  virtual void SetValue(const FieldType &value,
                        FieldTypeArray &values,
                        ContraptionID /*id*/) {
    CheckType(&value);
    FieldTypeT<pt::ptime> &pt_time = dynamic_cast<FieldTypeT<pt::ptime>&>(&*value);
    values[static_cast<int>(this->field_id_)].reset(new FieldTypeT<std::string>(
            pt::to_iso_string(pt_time.data)));
  }
 private:
  std::string default_value_;
  const std::string backend_name_;
};

class DateField : public DateFieldImpl {
 public:
  BOOST_PARAMETER_CONSTRUCTOR(
    DateField, (DateFieldImpl),
    tag,
    (required
      (name, (std::string))
    )
    (optional
      (is_readable, (bool))
      (is_writable, (bool))
      (backend_name, (std::string))
      (default_value, (T))
    )
  )
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_

#endif // DATE_FIELD_HPP_INCLUDED
