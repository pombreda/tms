#ifndef _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <typeinfo>
// boost
#include <boost/parameter.hpp>
// common
#include <contraption/field.hpp>
#include <contraption/record.hpp>
#include <contraption/contraption.hpp>
#include <contraption/filter.hpp>

namespace tms {
namespace common {
namespace contraption {

BOOST_PARAMETER_NAME(name)
BOOST_PARAMETER_NAME(is_readable)
BOOST_PARAMETER_NAME(is_writable)
BOOST_PARAMETER_NAME(backend_name)
BOOST_PARAMETER_NAME(default_value)

//------------------------------------------------------------
// SimpleFieldT class. Represents a simple mapping to db.
//------------------------------------------------------------
template<class T>
class SimpleFieldTImpl : public FieldT<T> {  
 public:  
  using FieldT<T>::CheckType;
  template <class ArgPack>
  SimpleFieldTImpl(ArgPack const &args) 
      throw(FieldException) :
      FieldT<T>(args[_name], 
                args[_is_readable | true], 
                args[_is_writable | true]),
      default_value_(args[_default_value | T()]),
      backend_name_(args[_backend_name | args[_name]]) {
  }

  virtual void GetReadRecords(FieldTypeArray &values, 
                              ContraptionID id,
                              std::vector<RecordP> &out) const {
    if (!values[(int)this->field_id_]) { 
      values[(int)this->field_id_].reset(new FieldTypeT<T>(default_value_));
      if (id != Contraption::kNewID) {
        out.push_back(RecordP(
            new RecordT<T>(backend_name_, &(dynamic_cast<FieldTypeT<T>*>(
                values[(int)this->field_id_].get())->data_))));
      }
    }
  }

  virtual void GetWriteRecords(FieldTypeArray &values, 
                               ContraptionID id,
                               std::vector<RecordP> &out) const {
    if (!values[(int)this->field_id_] 
        && id == Contraption::kNewID) {
      values[(int)this->field_id_].reset(new FieldTypeT<T>(default_value_));
    }
    if (values[(int)this->field_id_]) {
      out.push_back(RecordP(
          new RecordT<T>(backend_name_, &(dynamic_cast<FieldTypeT<T>*>(
              values[(int)this->field_id_].get())->data_))));
    }
  }

  const std::string backend_name() const
      throw () {
    return backend_name_;
  }

  virtual ~SimpleFieldTImpl() {}

 private:
  T default_value_;
  const std::string backend_name_; 
};

template <class T>
class SimpleFieldT : public SimpleFieldTImpl<T> {
 public:
  BOOST_PARAMETER_CONSTRUCTOR(
    SimpleFieldT, (SimpleFieldTImpl<T>),
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
