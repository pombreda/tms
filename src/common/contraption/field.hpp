#ifndef _TMS_COMMON_CONTRAPTION__FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <string>
#include <memory>
#include <contraption/model.hpp>
#include <contraption/filter.hpp>
#include <contraption/selector.hpp>


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
  virtual std::string name() const { return name_; }
  virtual bool is_private() const { return is_private_; }
  virtual bool CheckType(const FieldType *type) const = 0;
  friend class SimpleField;
  friend class DependantField;
  virtual ~Field() {}
  Field(const std::string &name, bool is_private = false) 
      throw(FieldException) : 
      name_(name),
      is_private_(is_private) {
    if (name.size() == 0) {
      throw FieldException("Field cannot be constructed from empty name");
    }
  }
 private:
  std::string name_;  
  bool is_private_;
};

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


class SimpleField : virtual public Field {  
 public:
  SimpleField(const std::string &name, bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_), 
      backend_name_(name) {}

  SimpleField(const std::string &name, const std::string &backend_name_,
         bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_), 
      backend_name_(backend_name_) {}
  virtual ~SimpleField() {}
 private:
  std::string backend_name_;  
};

template<class T>
class SimpleFieldT : virtual public SimpleField,
                     virtual public FieldT<T> {  
 public:
  SimpleFieldT(const std::string &name, bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_),
      SimpleField(name, is_private_),
      FieldT<T>(name, is_private_) {}

  SimpleFieldT(const std::string &name, const std::string &backend_name_,
         bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_),
      SimpleField(name, is_private_, backend_name_),
      FieldT<T>(name, is_private_){}
  
  virtual ~SimpleFieldT() {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
