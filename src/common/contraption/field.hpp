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
  friend class SimpleField;
  friend class DependantField;
  virtual ~Field() {}
 private:
  Field(const std::string &name, bool is_private = false) 
      throw(FieldException) : 
      name_(name),
      is_private_(is_private) {
    if (name.size() == 0) {
      throw FieldException("Field cannot be constructed from empty name");
    }
  }
  std::string name_;  
  bool is_private_;
};

class SimpleField : public Field {  
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
class SimpleFieldT : public SimpleField {  
 public:
  SimpleFieldT(const std::string &name, bool is_private_ = false)
      throw(FieldException) : 
      SimpleField(name, is_private_) {}

  SimpleFieldT(const std::string &name, const std::string &backend_name_,
         bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_, backend_name_) {}
  
  virtual ~SimpleFieldT() {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
