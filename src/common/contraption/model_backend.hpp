#ifndef _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// common
#include <contraption/model_fwd.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/contraption_fwd.hpp>
#include <contraption/model_backend_exception.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// ModelBackend class
//------------------------------------------------------------
class ModelBackend {
 public:
  virtual FieldType* GetField(FieldID field_id,
                              Contraption *contraption)
      throw(ModelBackendException) = 0;
  virtual void Save(Contraption *contraption)
      throw(ModelBackendException) = 0;
  virtual ~ModelBackend() {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
