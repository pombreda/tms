#ifndef _TMS_COMMON_CONTRAPTION__FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <string>
#include <memory>

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Field class. This is an abstract class.
//------------------------------------------------------------
class Field {  
  // ?
  Field(const std::string &name) : name_(name){}

  virtual Selector *ApplyFilter(const FieldFilter *filter) const = 0;

  void* GetValue(Model *model, ContraptionID id) const = 0;

  virtual std::string name() const { return name_; }
  virtual FieldType* type() const = 0; 

 private:
  std::string name_;  
};

template<class T>
class FieldT<T> {  
  // ?
  FieldT(const std::string &name) : Field(name), type_(new FieldTypeT<T>()){}
  
  virtual FieldType* type() const {return type_.get()}

 private:
  const auto_ptr<FieldType> type_;  
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
