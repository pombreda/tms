#ifndef _TMS_COMMON_CONTRAPTION__MODEL_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <string>
#include <vector>
#include <map>
// boost
#include <boost/shared_ptr.hpp>
// common
#include <contraption/model_backend.hpp>
#include <contraption/field_exception.hpp>
#include <contraption/model_backend_exception.hpp>
#include <contraption/field_fwd.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/contraption_fwd.hpp>
//
#include "model_fwd.hpp"

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Model class.
//------------------------------------------------------------
// ToDo - stl container interface?
// ToDo - move implementation
class Model { 
 public:
  size_t GetFieldNumber() const
      throw();
  
  FieldType* GetFieldValue(FieldID field_id, 
                           Contraption *contraption) const 
      throw(FieldException, ModelBackendException);
  
  FieldType* GetFieldValue(const std::string &field_name, 
                           Contraption *contraption) const
      throw(FieldException, ModelBackendException);

  const Field* GetField(FieldID field_id) const
      throw(FieldException);

  FieldID GetFieldID(const std::string &field_name) const
      throw(FieldException);
  
  void Save(Contraption *contraption) const
      throw(ModelBackendException);

  Model(const std::vector< Field* > &fields, 
        ModelBackend *backend)
      throw(FieldException);
  
  Model(const std::vector< Field* > &fields, 
        boost::shared_ptr<ModelBackend> backend)
      throw(FieldException);

 private:  
  void InitFields(const std::vector< Field* > &fields)
      throw(FieldException);
  Model() : 
      fields_(0), 
      fields_by_name_(), 
      backend_() {}
  std::vector< boost::shared_ptr<Field> > fields_; 
  std::map<std::string, FieldID> fields_by_name_;
  boost::shared_ptr<ModelBackend> backend_;
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_HPP_
