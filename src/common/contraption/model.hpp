#ifndef _TMS_COMMON_CONTRAPTION__MODEL_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include<string>
#include<map>
#include<contraption/field.hpp>

typedef size_t FieldID;

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Model class. 
//------------------------------------------------------------
// ToDo - stl container interface?
// ToDO - move implementation
class Model {
 public:
  size_t GetFieldNumber() {
    return fields_.size();
  }
  
  Field* GetField(FieldID field_id) {    
    if (id > GetFieldNumber()) {
      //oops
    }
    return fields_[field_id];
  }

  Field* GetField(const std::string &field_name) {    
    return GetField(GetFieldID(field));
  }

  FieldID GetFieldID(const std::string &field_name) {
    std::map::const_iterator it = fields_by_name_.find(field);
    if (it == fields_by_name_.end()) {
      //oops
    }
    return *it;
  }
  
 protected:
  void AddField(Field *field);
 private:
  std::vector< std::auto_ptr<Field*> > fields_; 
  std::map<std::string, FieldID> fields_by_name_;
}
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_HPP_
