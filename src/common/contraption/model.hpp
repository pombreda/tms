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

using boost::intrusive_ptr_add_ref;
using boost::intrusive_ptr_release;

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

  const Field* GetField(const std::string &field_name) const
      throw(FieldException);

  ContraptionP New()
      throw();

  ContraptionArrayP All()
      throw(ModelBackendException);

  ContraptionArrayP Empty()
      throw(ModelBackendException);

  ContraptionArrayP Filter(FilterCP filter)
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

  Model(ModelBackend *backend);

  Model(ModelBackendP backend);

  Model(const std::vector< Field* > &fields,
        ModelBackend *backend)
      throw(FieldException);

  Model(const std::vector< Field* > &fields,
        ModelBackendP backend)
      throw(FieldException);

  void InitFields(const std::vector< Field* > &fields)
      throw(FieldException);

  void CheckInit() const
      throw(FieldException);
  bool IsInitialized() const
      throw();

  virtual ~Model();
  friend void boost::intrusive_ptr_add_ref(Model* model);
  friend void boost::intrusive_ptr_release(Model* model);
  friend class Contraption;
 private:
  FieldTypeP GetFieldValue(FieldID field_id,
                           FieldTypeArray &values,
                           ContraptionID id) const
      throw(FieldException, ModelBackendException);

  void SetFieldValue(FieldID field_id,
                     const FieldType &value,
                     FieldTypeArray &values,
                     ContraptionID id) const
       throw(FieldException);

  void Save(FieldTypeArray &values, ContraptionID &id) const
      throw(ModelBackendException);

  void Delete(ContraptionID &id) const
      throw(ModelBackendException);

  Model() :
      fields_(0),
      fields_by_name_(),
      backend_(),
      initialized_(),
      ptr_count_(0) {}
  std::vector< boost::shared_ptr<Field> > fields_;
  std::map<std::string, FieldID> fields_by_name_;
  boost::shared_ptr<ModelBackend> backend_;
  bool initialized_;
 protected:
  size_t ptr_count_;
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
