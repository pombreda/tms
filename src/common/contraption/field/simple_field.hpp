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
#include <contraption/selector.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/selector/compare_selector.hpp>

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
      field_id_(9999),
      default_value_(args[_default_value | T()]),
      backend_name_(args[_backend_name | args[_name]]) {
  }

  virtual SelectorP GetSelector(Filter *filter) const
      throw(FieldException) {    
    {
      CompareFilter *cfilter = 
          dynamic_cast<CompareFilter *>(filter);
      if (cfilter) {
        return SelectorP(new CompareSelector(backend_name_, 
                                             cfilter->type()));
      }
    }
    return Field::GetSelector(filter);
  }

  virtual void Initialize(Model *model) {
    field_id_ = model->GetFieldID(this->name());
  }
  
  virtual void GetReadRecords(Contraption *contraption, 
                              std::vector<RecordP> &out) const {
    if (!contraption->values_[(int)field_id_]) {
      contraption->values_[(int)field_id_].reset(new FieldTypeT<T>(default_value_));
      if (contraption->id_ != Contraption::kNewID) {
        out.push_back(RecordP(
            new RecordT<T>(backend_name_, &(dynamic_cast<FieldTypeT<T>*>(
                contraption->values_[(int)field_id_].get())->data_))));
      }
    }
  }

  virtual void GetWriteRecords(Contraption *contraption, 
                          std::vector<RecordP> &out) const {
    if (!contraption->values_[(int)field_id_] 
        && contraption->id_ == Contraption::kNewID) {
      contraption->values_[(int)field_id_].reset(new FieldTypeT<T>(default_value_));
    }
    if (contraption->values_[(int)field_id_]) {
      out.push_back(RecordP(
          new RecordT<T>(backend_name_, &(dynamic_cast<FieldTypeT<T>*>(
              contraption->values_[(int)field_id_].get())->data_))));
    }
  }

  virtual ~SimpleFieldTImpl() {}
 private:
  FieldID field_id_;
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
