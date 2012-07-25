#ifndef _TMS_COMMON_CONTRAPTION__SERVER_MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__SERVER_MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <string>
#include <memory>
#include <vector>
// common
#include <contraption/model_backend.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/model_fwd.hpp>
#include <contraption/contraption_fwd.hpp>
#include <protocol/client.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// ServerModelBackend class.
//------------------------------------------------------------
class ServerModelBackend : public ModelBackend {
 public:
  virtual void ReadRecords(
      const std::vector<RecordP> &records,
      ContraptionID id)
      throw(ModelBackendException);      

  virtual void WriteRecords(
      const std::vector<RecordP> &records,
      ContraptionID &id)
      throw(ModelBackendException);      
  
  virtual void InitSchema(
      const std::vector<RecordP> &records)
      throw(ModelBackendException);

  virtual void DeleteEntry(
      ContraptionID &id)
      throw(ModelBackendException);
  
  virtual std::auto_ptr< std::vector<ContraptionID> > Select(
      FilterCP filter)
      throw(ModelBackendException);
  
  ServerModelBackend(const protocol::ClientP client, 
                     const std::string &table_name
                     ) 
      throw(ModelBackendException);  
 private:
  protocol::ClientP client_;
  std::string table_name_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SERVER_MODEL_BACKEND_HPP_
