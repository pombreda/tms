#ifndef _TMS_COMMON_CONTRAPTION__FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <string>
#include <memory>
#include <sstream>
// boost
#ifndef BOOST_PARAMETER_MAX_ARITY
#define BOOST_PARAMETER_MAX_ARITY 10
#endif // BOOST_PARAMETER_MAX_ARITY
#include <boost/parameter.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/filter.hpp>
#include <contraption/field_type.hpp>
#include <contraption/record.hpp>
#include <contraption/contraption_fwd.hpp>


namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Field class. This is an abstract class.
// Do not inherit this class directly.
// If you have to extend it inherit SimpleField
// or DependantField instead.
//------------------------------------------------------------
class Field {  
 public:
  //------------------------------------------------------------
  // Interface.
  //------------------------------------------------------------
  virtual std::string name() const { return name_; }
  virtual bool IsReadable() const { return is_readable_; }
  virtual bool IsWritable() const { return is_writable_; }
  Field(const std::string &name, bool is_readable, bool is_writable) 
      throw(FieldException) : 
      field_id_(9999),
      is_readable_(is_readable),
      is_writable_(is_writable),
      name_(name) {
    if (name.size() == 0) {
      throw FieldException("Field cannot be constructed from empty name.");
    }
  }

  //------------------------------------------------------------
  // Implementation. Do not use this methods.
  //------------------------------------------------------------
  virtual void Initialize(Model *model) {
    field_id_ = model->GetFieldID(this->name());
  }
  
  virtual void GetReadRecords(FieldTypeArray &/*values*/, 
                              ContraptionID /*id*/,
                              std::vector<RecordP> &/*out*/
                              ) const {
  }

  virtual void GetWriteRecords(FieldTypeArray &/*values*/, 
                               ContraptionID /*id*/,
                               std::vector<RecordP> &/*out*/
                               ) const {
  }
  
  virtual bool CheckType(const FieldType *type) const = 0;
  virtual void FinalizeGet(FieldTypeArray &/*values*/, 
                           ContraptionID /*id*/) const {
  }
  virtual void FinalizeSave(FieldTypeArray &/*values*/,
                            ContraptionID /*id*/) const {
  }
  virtual FieldTypeP GetValue(FieldTypeArray &values, ContraptionID /*id*/) {
    return FieldTypeP(values[static_cast<int>(this->field_id_)]->Duplicate());
  }
  virtual void SetValue(const FieldType &value, 
                        FieldTypeArray &values,
                        ContraptionID /*id*/) {
    CheckType(&value);
    values[static_cast<int>(this->field_id_)].reset(value.Duplicate());
  }
  virtual FieldID field_id() const { return field_id_; }

  virtual ~Field() {}

 protected:
  FieldID field_id_;
  const bool is_readable_;
  const bool is_writable_;
 private: 
  const std::string name_;
};

//------------------------------------------------------------
// FieldT class. All non-abstract fields classes must inherit 
// this class.
//------------------------------------------------------------
template<class T>
class FieldT : public Field {  
 public:
  FieldT(std::string name, bool is_readable, bool is_writable)
      throw(FieldException) :
      Field(name, is_readable, is_writable) {}
  virtual bool CheckType(const FieldType *type) const {
    return dynamic_cast<const FieldTypeT<T>*>(type);
  }
  virtual ~FieldT() {}
};

BOOST_PARAMETER_NAME(name)
BOOST_PARAMETER_NAME(is_readable)
BOOST_PARAMETER_NAME(is_writable)

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
