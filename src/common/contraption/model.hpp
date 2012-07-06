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
#include <contraption/contraption_array_fwd.hpp>
//
#include "model_fwd.hpp"

namespace boost {
inline void intrusive_ptr_add_ref(tms::common::contraption::Model* model);
inline void intrusive_ptr_release(tms::common::contraption::Model* model);
}

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
  
  FieldID GetFieldID(const std::string &field_name) const
      throw(FieldException);

  const Field* GetField(FieldID field_id) const
      throw(FieldException);

  ContraptionP New()
      throw();

  ContraptionArrayP All()
      throw(ModelBackendException);

  // This method will init backend to work with models.
  // For example - for SOCIModelBackand it
  // creates corresponding table.
  // In general it will be called once, when your 
  // application is installed.
  // Attention! This method can drop your data.
  // At least for SOCIModelBackand it does.
  void InitSchema()
      throw(ModelBackendException);

  Model(const std::vector< Field* > &fields, 
        ModelBackend *backend)
      throw(FieldException);
  
  Model(const std::vector< Field* > &fields, 
        boost::shared_ptr<ModelBackend> backend)
      throw(FieldException);
  
  friend void boost::intrusive_ptr_add_ref(Model* model);
  friend void boost::intrusive_ptr_release(Model* model);
  friend class Contraption;
 private:  
  FieldTypeP GetFieldValue(FieldID field_id, 
                           FieldTypeArray &values,
                           ContraptionID id) const 
      throw(FieldException, ModelBackendException);
  
  FieldTypeP GetFieldValue(const std::string &field_name, 
                           FieldTypeArray &values,
                           ContraptionID id) const
      throw(FieldException, ModelBackendException);

  void Save(FieldTypeArray &values, ContraptionID &id) const
      throw(ModelBackendException);

  void Delete(ContraptionID &id) const
      throw(ModelBackendException);

  void SaveHandle(const std::vector<ContraptionP> &save, 
                  const std::vector<ContraptionP> &remove) const
      throw(ModelBackendException);
  void InitFields(const std::vector< Field* > &fields)
      throw(FieldException);
  Model() : 
      ptr_count_(0),
      fields_(0), 
      fields_by_name_(), 
      backend_() {}
  size_t ptr_count_;
  std::vector< boost::shared_ptr<Field> > fields_; 
  std::map<std::string, FieldID> fields_by_name_;
  boost::shared_ptr<ModelBackend> backend_;
};
}
}
}
namespace boost {
inline void intrusive_ptr_add_ref(tms::common::contraption::Model* model) {
  ++model->ptr_count_;
}

inline void intrusive_ptr_release(tms::common::contraption::Model* model) {
  if(!(--model->ptr_count_))
    delete model;
}
}

#endif // _TMS_COMMON_CONTRAPTION__MODEL_HPP_
