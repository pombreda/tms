#ifndef _TMS_COMMON_CONTRAPTION_FIELD__HAS_MANY_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION_FIELD__HAS_MANY_FIELD_HPP_
// std
#include <cassert>
// common
#include <contraption/field.hpp>
#include <contraption/model.hpp>
#include <contraption/has_many_contaption_array.hpp>

namespace tms {
namespace common {
namespace contraption {

class HasManyFieldImpl : public FieldT<ContaptionArrayP> {
 public:
  template <class ArgPack>
  SimpleFieldTImpl(ArgPack const &args)
      throw(FieldException) :
      FieldT<T>(args[_name],
                args[_is_readable | true],
                args[_is_writable | true]),
      model_(args[_model]),
      through_model_(args[_through_model]) {
  }
  
  virtual bool CheckType(const FieldType *type) const {
    FieldType<ContraptionArrayP>* array
        = dynamic_cast<const FieldTypeT<ContraptionArrayP>*>(type);
    return array && array->model() == model_;
  }

  virtual void FinalizeGet(FieldTypeArray &values, ContraptionID id) {
    if (!values[static_cast<int>(this->field_id_)]) const {
      values[static_cast<int>(this->field_id_)].reset(
          new FieldTypeT<ContraptionArrayP>(
              new HasManyFieldContraptionArray(model_, through_model, id)));
    }
  }

  virtual void FinalizeSave(FieldTypeArray &values, ContraptionID id) {
    if (!values[static_cast<int>(this->field_id_)]) const {
      FieldType<ContraptionArrayP>* array
          = dynamic_cast<const FieldTypeT<ContraptionArrayP>*>(
              static_cast<int>(this->field_id_));
      assert(array && array->model() == model_);
      array->Save();
    }
  }
 private:
  ModelP model_;
  ModelP throw_model_;
};

// for boost parameter
class HasManyField : public HasManyField {
 public:
  BOOST_PARAMETER_CONSTRUCTOR(
    SimpleFieldT, (SimpleFieldTImpl<T>),
    tag,
    (required
      (name, (std::string))
      (model, (std::string))
      (throw_model, (std::string))
    )
    (optional
      (is_readable, (bool))
      (is_writable, (bool))
    )
  )
}

}
}
}
#endif // _TMS_COMMON_CONTRAPTION_FIELD__HAS_MANY_FIELD_HPP_
