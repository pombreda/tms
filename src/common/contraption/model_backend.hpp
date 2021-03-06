#ifndef _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// stl
#include <vector>
// boost
#include <boost/shared_ptr.hpp>
// common
#include <contraption/model_backend_exception.hpp>
#include <contraption/record.hpp>
#include <contraption/contraption_fwd.hpp>
#include <contraption/filter.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// ModelBackend class
//------------------------------------------------------------
class ModelBackend {
 public:
  virtual void ReadRecords(
      const std::vector<RecordP> &records,
      ContraptionID id)
      throw(ModelBackendException) = 0;
  virtual void WriteRecords(
      const std::vector<RecordP> &records,
      ContraptionID &id)
      throw(ModelBackendException) = 0;
  virtual void InitSchema(
      const std::vector<RecordP> &records)
      throw(ModelBackendException) = 0;
  virtual void DeleteEntry(
      ContraptionID &id)
      throw(ModelBackendException) = 0;
  virtual std::auto_ptr<std::vector<ContraptionID> > Select(
      FilterCP filter)
      throw(ModelBackendException) = 0; 

  virtual ~ModelBackend() {}
};

typedef boost::shared_ptr<ModelBackend> ModelBackendP;

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
