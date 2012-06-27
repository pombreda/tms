#ifndef _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// stl
#include <vector>
// common
#include <contraption/model_backend_exception.hpp>
#include <contraption/record.hpp>
#include <contraption/contraption_fwd.hpp>
#include <contraption/selector.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// ModelBackend class
//------------------------------------------------------------
class ModelBackend {
 public:
  virtual void ReadRecords(
      std::vector<Record*> records,
      ContraptionID id)
      throw(ModelBackendException) = 0;
  virtual void WriteRecords(
      std::vector<Record*> records,
      ContraptionID &id)
      throw(ModelBackendException) = 0;
  virtual std::auto_ptr<std::vector<ContraptionID> > Select(const Selector &selector) {}

  virtual ~ModelBackend() {}
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
