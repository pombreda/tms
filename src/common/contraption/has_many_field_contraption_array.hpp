#ifndef _TMS_COMMON_CONTRAPTION__HAS_MANY_FIELD_CONTRAPTION_ARRAY_HPP_
#define _TMS_COMMON_CONTRAPTION__HAS_MANY_FIELD_CONTRAPTION_ARRAY_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <vector>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/signals/connection.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/field/simple_field.hpp>

namespace tms {
namespace common {
namespace contraption {
// ToDo tests!!!
class HasManyFieldContraptionArray : public ContraptionArray {
 public:
  virtual void Save();
  friend class HasManyFieldImpl;
  virtual ContraptionArrayP Duplicate();
 private:
  HasManyFieldContraptionArray(
      ModelP model,
      ModelP through_model,
      const IntField *id_column,
      const IntField *other_id_column,
      ContraptionID id)
      throw();
  ModelP through_model_;
  virtual void push_back(ContraptionP contraption);
  const IntField *id_column_;
  const IntField *other_id_column_;
  ContraptionID id_;
  std::vector<ContraptionP> to_add_; 
};

typedef boost::shared_ptr<HasManyFieldContraptionArray> HasManyFieldContraptionArrayP;

//------------------------------------------------------------
// FieldTypeT<T> class. Wrapper for a c++ type.
//------------------------------------------------------------
template<>
class FieldTypeT<ContraptionArrayP> : public FieldType {
 public:
  FieldTypeT() : data_() {}
  FieldTypeT(ContraptionArrayP data) : data_(data) {}
  virtual ~FieldTypeT(){}
  ContraptionArrayP data() const { return data_; }
  ContraptionArrayP& data() { return data_; }
  void set_data(const ContraptionArrayP& data) { data_ = data; }
  virtual FieldTypeT<ContraptionArrayP>* Duplicate() const { return new FieldTypeT<ContraptionArrayP>(data_); } 
  virtual FieldTypeT<ContraptionArrayP>* Clone() const { return new FieldTypeT<ContraptionArrayP>(data_->Duplicate()); } 
  inline std::string ToString() const;
  friend class SimpleFieldTImpl<ContraptionArrayP>;
 private:
  ContraptionArrayP data_;
};


inline std::string FieldTypeT<ContraptionArrayP>::ToString() const {
  return rtti::TypeID(*this).name();
}


}
}
}

#endif // _TMS_COMMON_CONTRAPTION__HAS_MANY_FIELD_CONTRAPTION_ARRAY_HPP_
