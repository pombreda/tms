#ifndef _TMS_COMMON_CONTRAPTION__FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <string>
#include <memory>
// common

#include <contraption/model.hpp>
#include <contraption/filter.hpp>
#include <contraption/selector.hpp>
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
  virtual bool is_private() const { return is_private_; }
  friend class SimpleField;
  friend class DependantField;
  Field(const std::string &name, bool is_private = false) 
      throw(FieldException) : 
      name_(name),
      is_private_(is_private) {
    if (name.size() == 0) {
      throw FieldException("Field cannot be constructed from empty name.");
    }
  }

  //------------------------------------------------------------
  // Implementation. Do not use this methods.
  //------------------------------------------------------------
  virtual void Initialize(Model *model) {model = model;}
  virtual void GetReadRecords(Contraption *contraption, 
                              std::vector<RecordP> &out
                              ) const = 0;
  virtual void GetWriteRecords(Contraption *contraption, 
                               std::vector<RecordP> &out
                               ) const = 0;
  
  virtual bool CheckType(const FieldType *type) const = 0;
  
  virtual ~Field() {}
 private:

  const std::string name_;  
  const bool is_private_;
};

//------------------------------------------------------------
// FieldT class. All non-abstract fiels classes must inherit 
// this class.
//------------------------------------------------------------
template<class T>
class FieldT : virtual public Field {  
 public:
  FieldT(const std::string &name, bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_) {}
  virtual bool CheckType(const FieldType *type) const {
    return dynamic_cast<const FieldTypeT<T>*>(type);
  }
  virtual ~FieldT() {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
