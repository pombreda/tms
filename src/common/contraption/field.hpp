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
 private:
  Field(const std::string &name, bool is_private = false) : 
      name_(name),
      is_private_(is_private) {}
  std::string name_;  
  bool is_private_;
};

template<class T>
class SimpleField {  
 public:
  FieldT(const std::string &name, bool is_private_ = false) : 
      Field(name, is_private_), 
      backend_name_(name) {}

  FieldT(const std::string &name, const std::string &backend_name_,
         bool is_private_ = false) : 
      Field(name, is_private_), 
      backend_name_(backend_name_name) {}
  
  virtual FieldType* type() const {return type_.get()}
  virtual ~FieldT() {}
 private:
  std::string backend_name_;  
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
