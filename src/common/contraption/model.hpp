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
  
  const Field* GetField(FieldID field_id) const
      throw(FieldException);

  FieldID GetFieldID(const std::string &field_name) const
      throw(FieldException);
  
  ContraptionArrayP All()
      throw(ModelBackendException) {return GetContraptions();}

  ContraptionArrayP GetContraptions()
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
  void GetFieldValue(FieldID field_id, 
                     Contraption *contraption,
                     ContraptionID id) const 
      throw(FieldException, ModelBackendException);
  
  void GetFieldValue(const std::string &field_name, 
                     Contraption *contraption,
                     ContraptionID id) const
      throw(FieldException, ModelBackendException);

  void Save(Contraption *contraption, ContraptionID &id) const
      throw(ModelBackendException);

  void Delete(ContraptionID &id) const
      throw(ModelBackendException);

  void SaveHandle(const std::vector<ContraptionP> &save, 
                  const std::vector<ContraptionP> &remove)
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
