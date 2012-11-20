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
#include <contraption/field/has_many_field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/model.hpp>
#include <contraption/has_many_field_contraption_array.hpp>
#include <contraption/model_backend_exception.hpp>
#include <iostream>
using namespace std; // oops
namespace tms {
namespace common {
namespace contraption {

BOOST_PARAMETER_NAME(field_column)
BOOST_PARAMETER_NAME(has_many_column)
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
    has_many_column_(0) {
    try {
      has_many_column_ = args[_has_many_column];
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
  
  virtual void FinalizeGet(FieldTypeArray &values, ContraptionID /*id*/) const {
    if (!values[static_cast<int>(this->field_id_)]) {
      T val;
      if (values[static_cast<int>(has_many_column_->field_id())]) {
	const FieldTypeT<ContraptionArrayP>* type
	  = dynamic_cast<const FieldTypeT<ContraptionArrayP>*>(
		 &*values[static_cast<int>(has_many_column_->field_id())]);
	assert(type);
	HasManyFieldContraptionArray* array
          = dynamic_cast<HasManyFieldContraptionArray*>(&*type->data());      
	assert(array);
	if (array->size() > 0) {
	  val = array->at(0)->Get<T>(field_column_);
	  }
      }
      values[static_cast<int>(this->field_id_)].reset(
          new FieldTypeT<T>(val));
    }
  }
  
  virtual void FinalizeSave(FieldTypeArray &/*values*/, ContraptionID /*id*/) const {
  }
 private:
  std::string field_column_;
  const HasManyField *has_many_column_;
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
      (has_many_column, (HasManyField*))
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
