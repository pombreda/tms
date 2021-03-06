#ifndef _TMS_COMMON_CONTRAPTION_FIELD__HAS_MANY_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION_FIELD__HAS_MANY_FIELD_HPP_
#include <config.hpp>
// std
#include <cassert>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/parameter.hpp>
// common
#include <contraption/field.hpp>
#include <contraption/model.hpp>
#include <contraption/has_many_field_contraption_array.hpp>
#include <contraption/model_backend_exception.hpp>
#include <iostream>
using namespace std; // oops
namespace tms {
namespace common {
namespace contraption {


BOOST_PARAMETER_NAME(through_model)
BOOST_PARAMETER_NAME(model)
BOOST_PARAMETER_NAME(id_column)
BOOST_PARAMETER_NAME(other_id_column)


class HasManyFieldImpl : public FieldT<ContraptionArrayP> {
 public:
  template <class ArgPack>
  HasManyFieldImpl(ArgPack const &args)
      throw(FieldException) :
      FieldT<ContraptionArrayP>(args[_name],
                                args[_is_readable | true],
                                args[_is_writable | true]),
      model_(args[_model]),
      through_model_(args[_through_model]),
      id_column_(args[_id_column | "this_id"]),
      other_id_column_(args[_other_id_column | "other_id"]) {
    try {
    } catch (FieldException &e) {
      throw ModelBackendException(&e);
    }
  }
  
  virtual bool CheckType(const FieldType *type) const {
    const FieldTypeT<ContraptionArrayP>* array
        = dynamic_cast<const FieldTypeT<ContraptionArrayP>*>(type);
    return array && &*array->data()->model() == &model_;
  }

  virtual void FinalizeGet(FieldTypeArray &values, ContraptionID id) const {
    const IntField *id_column
          = dynamic_cast<const IntField*>(
              through_model_.GetField(id_column_));
    const IntField *other_id_column
          = dynamic_cast<const IntField*>(
              through_model_.GetField(other_id_column_));
    if (!values[static_cast<int>(this->field_id_)]) {
      ContraptionArrayP val =  ContraptionArrayP(
          new HasManyFieldContraptionArray(ModelP(const_cast<Model*>(&model_)),
                                           ModelP(const_cast<Model*>(
                                               &through_model_)), 
                                           id_column,
                                           other_id_column,
                                           id));
      values[static_cast<int>(this->field_id_)].reset(
          new FieldTypeT<ContraptionArrayP>(val));
    }
  }
  
  virtual void FinalizeSave(FieldTypeArray &values, ContraptionID id) const {
    if (values[static_cast<int>(this->field_id_)]) {
      const FieldTypeT<ContraptionArrayP>* type
          = dynamic_cast<const FieldTypeT<ContraptionArrayP>*>(
              &*values[static_cast<int>(this->field_id_)]);
      assert(type);
      HasManyFieldContraptionArray* array
          = dynamic_cast<HasManyFieldContraptionArray*>(&*type->data());      
      assert(array && &*array->model() == &model_);
      array->id_ = id;
      array->Save();
    }
  }
 private:
  Model& model_;
  Model& through_model_;
  std::string id_column_;
  std::string other_id_column_;
};

// for boost parameter
class HasManyField : public HasManyFieldImpl {
 public:
  BOOST_PARAMETER_CONSTRUCTOR(
    HasManyField, (HasManyFieldImpl),
    tag,
    (required
      (name, (std::string))
      (model, (Model&))
      (through_model, (Model&))
    )
    (optional
      (is_readable, (bool))
      (is_writable, (bool))
      (id_column, (std::string))
      (other_id_column, (std::string))
    )
  )
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION_FIELD__HAS_MANY_FIELD_HPP_
