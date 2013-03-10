#ifndef _TMS_COMMON_CONTRAPTION_FIELD__PROXY_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION_FIELD__PROXY_FIELD_HPP_
#include <config.hpp>
// std
#include <cassert>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/parameter.hpp>
// common
#include <contraption/field.hpp>
#include <contraption/field/has_one_field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/model.hpp>
#include <contraption/model_backend_exception.hpp>
#include <iostream>
using namespace std; // oops
namespace tms {
namespace common {
namespace contraption {

BOOST_PARAMETER_NAME(field_column)
BOOST_PARAMETER_NAME(has_one_column)
template<class T>
class ProxyFieldImpl : public FieldT<T> {
public:
  template <class ArgPack>
  ProxyFieldImpl(ArgPack const &args)
    throw(FieldException) :
    FieldT<T>(args[_name],
              args[_is_readable | true],
              false),
    field_column_(),
    has_one_column_(0) {
    try {
      has_one_column_ = args[_has_one_column];
      field_column_ = args[_field_column];
    } catch (FieldException &e) {
      throw ModelBackendException(&e);
    }
  }
  
  virtual bool CheckType(const FieldType *type) const {
    const FieldTypeT<T>* array
      = dynamic_cast<const FieldTypeT<T>*>(type);
    return array;
  }
  
  virtual void GetReadRecords(FieldTypeArray &values,
                              ContraptionID id,
                              std::vector<RecordP> &out) const {
    has_one_column_->GetReadRecords(values, id, out);
  }

  virtual void FinalizeGet(FieldTypeArray &values, ContraptionID id) const {
    if (!values[static_cast<int>(this->field_id_)]) {
      has_one_column_->FinalizeGet(values, id);
      T val;
      const FieldTypeT<ContraptionP>* type
          = dynamic_cast<const FieldTypeT<ContraptionP>*>(
              &*values[static_cast<int>(has_one_column_->field_id())]);
      assert(type);
      ContraptionP contraption = type->data();
      if (contraption) {
	val = contraption->Get<T>(field_column_);
      }
      values[static_cast<int>(this->field_id_)].reset(
          new FieldTypeT<T>(val));
    }
  }
  
  virtual void FinalizeSave(FieldTypeArray &/*values*/, ContraptionID /*id*/) const {
  }
 private:
  std::string field_column_;
  const HasOneField *has_one_column_;
};

// for boost parameter
template<class T>
class ProxyField : public ProxyFieldImpl<T> {
 public:
  BOOST_PARAMETER_CONSTRUCTOR(
    ProxyField, (ProxyFieldImpl<T>),
    tag,
    (required
      (name, (std::string))
      (has_one_column, (HasOneField*))
      (field_column, (std::string))
    )
    (optional
      (is_readable, (bool))
    )
  )
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION_FIELD__PROXY_FIELD_HPP_
