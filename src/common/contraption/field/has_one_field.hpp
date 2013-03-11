#ifndef _TMS_COMMON_CONTRAPTION_FIELD__HAS_ONE_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION_FIELD__HAS_ONE_FIELD_HPP_
#include <config.hpp>
// std
#include <cassert>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/parameter.hpp>
// common
#include <contraption/field.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <contraption/field/has_many_field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/model.hpp>
#include <contraption/model_backend_exception.hpp>
#include <iostream>
using namespace std; // oops
namespace tms {
namespace common {
namespace contraption {


class HasOneFieldImpl : public FieldT<ContraptionP> {
 public:
  template <class ArgPack>
  HasOneFieldImpl(ArgPack const &args)
      throw(FieldException) :
      FieldT<ContraptionP>(args[_name],
                                args[_is_readable | true],
                                args[_is_writable | true]),
      model_(args[_model]),
      other_id_column_() {
    try {
      other_id_column_ = args[_other_id_column];
    } catch (FieldException &e) {
      throw ModelBackendException(&e);
    }
  }
  
  virtual bool CheckType(const FieldType *type) const {
    const FieldTypeT<ContraptionP>* contraption
        = dynamic_cast<const FieldTypeT<ContraptionP>*>(type);
    return contraption && 
        (!contraption->data() ||
         &*contraption->data()->model() == &model_);
  }


  virtual void GetReadRecords(FieldTypeArray &values,
                              ContraptionID id,
                              std::vector<RecordP> &out) const {
    other_id_column_->GetReadRecords(values, id, out);
  }

  virtual void GetWriteRecords(FieldTypeArray &values,
                               ContraptionID id,
                               std::vector<RecordP> &out) const {
    if (values[static_cast<int>(this->field_id_)]) {
      const FieldTypeT<ContraptionP>* type
          = dynamic_cast<const FieldTypeT<ContraptionP>*>(
              &*values[static_cast<int>(this->field_id_)]);
      assert(type);
      ContraptionP contraption = type->data();      
      int ans = Contraption::kNewID;
      if (contraption) {
        assert(contraption->model() == &model_);
        ans = contraption->Get<int>("id");
      }
      if (values[static_cast<int>(other_id_column_->field_id())]) {
        FieldTypeT<int>* type
            = dynamic_cast<FieldTypeT<int>*>(
                &*values[static_cast<int>(other_id_column_->field_id())]);
        assert(type);        
        type->set_data(ans);
      } else {
        values[static_cast<int>(other_id_column_->field_id())].reset(
            new FieldTypeT<int>(ans));
      }
    }
    other_id_column_->GetWriteRecords(values, id, out);
  }


  virtual void FinalizeGet(FieldTypeArray &values, ContraptionID /*id*/) const {    
    if (!values[static_cast<int>(this->field_id_)]) {
      const IntField *id_column
          = dynamic_cast<const IntField*>(
              model_.GetField("id"));
      assert(values[static_cast<int>(other_id_column_->field_id())]);
      const FieldTypeT<int>* type
          = dynamic_cast<const FieldTypeT<int>*>(
              &*values[static_cast<int>(other_id_column_->field_id())]);
      assert(type);
      int other_id = type->data();
      ContraptionArrayP array = model_.Filter(Compare(id_column, kEqual, other_id));
      assert(array);
      if (array->size() == 0) {
        values[static_cast<int>(this->field_id_)].reset(
            new FieldTypeT<ContraptionP>(ContraptionP()));        
      } else {
        values[static_cast<int>(this->field_id_)].reset(
            new FieldTypeT<ContraptionP>(array->at(0)));        
      }
    }
  }
  
  virtual void FinalizeSave(FieldTypeArray &/*values*/, ContraptionID /*id*/) const {
  }

 private:
  Model& model_;
  const IntField *other_id_column_;
};

// for boost parameter
class HasOneField : public HasOneFieldImpl {
 public:
  BOOST_PARAMETER_CONSTRUCTOR(
    HasOneField, (HasOneFieldImpl),
    tag,
    (required
      (name, (std::string))
      (model, (Model&))
      (other_id_column, (IntField*))
    )
    (optional
      (is_readable, (bool))
      (is_writable, (bool))
    )
  )
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION_FIELD__HAS_ONE_FIELD_HPP_
