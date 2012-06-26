#ifndef _TMS_COMMON_CONTRAPTION__MODEL_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <string>
#include <map>
#include <contraption/field.hpp>

namespace tms {
namespace common {
namespace contraption {

typedef size_t FieldID;

//------------------------------------------------------------
// Model class.
//------------------------------------------------------------
// ToDo - stl container interface?
// ToDo - move implementation
class Model {
 public:
  size_t GetFieldNumber() const
      throw();
  
  FieldType* GetField(FieldID field_id, ContraptionID id) const 
      throw(FieldExcepton, ModelBackendException);

  FieldType* GetField(const std::string &field_name, 
                      ContraptionID id) const
      throw(FieldExcepton, ModelBackendException);

  bool FieldType* IsPrivate(FieldID field_id) const
      throw(FieldExcepton);

  FieldID GetFieldID(const std::string &field_name) const
      throw(FieldExcepton);

  void Save(Contraption *contraption) const
      throw(ModelBackendException);

  Model(const std::vector< Field* > &fields, 
        const ModelBackend *backend) : 
      fields_(fields.size()),
      backend_(backend)
      throw();

 protected:  
  Model():fields_(0), fields_by_name_() {}
  std::vector< boost::shared_ptr<Field> > fields_; 
  std::map<std::string, FieldID> fields_by_name_;
  boost::shared_ptr< ModelBackend > backend_;
}
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_HPP_
