#ifndef _TMS_COMMON_CONTRAPTION__FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <string>
#include <memory>
#include <sstream>
// common
#include <contraption/model.hpp>
#include <contraption/filter.hpp>
#include <contraption/field_type.hpp>
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
  virtual bool IsPrivate() const { return is_private_; }
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

  virtual SelectorP GetSelector(Filter *filter) const
      throw(FieldException) {
    std::ostringstream msg;
    msg << "Unsupported filter '" << typeid(*filter).name()
        << "' for field " <<  name_ << ".'";
    throw FieldException(msg.str());
    
    throw FieldException("Unsupported Filter");
  }

  virtual ~Field() {}
 private:

  const std::string name_;  
  const bool is_private_;
};

//------------------------------------------------------------
// FieldT class. All non-abstract fields classes must inherit 
// this class.
//------------------------------------------------------------
template<class T>
class FieldT : public Field {  
 public:
  FieldT(std::string name, bool is_private)
      throw(FieldException) :
      Field(name, is_private) {}
  virtual bool CheckType(const FieldType *type) const {
    return dynamic_cast<const FieldTypeT<T>*>(type);
  }
  virtual ~FieldT() {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_HPP_
