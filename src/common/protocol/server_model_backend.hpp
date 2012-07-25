#ifndef _TMS_COMMON_CONTRAPTION__SOCI_MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__SOCI_MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// soci
#include <soci/soci.h>
// common
#include <contraption/model_backend.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/model_fwd.hpp>
#include <contraption/contraption_fwd.hpp>


namespace tms {
namespace common {
namespace prototol {
//------------------------------------------------------------
// ServerModelBackend class.
//------------------------------------------------------------
class ServerModelBackend : public contraption::ModelBackend {
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
  
  SOCIModelBackend(const SOCIDBScheme &scheme, 
                   const std::string &table_name,
                   std::string id_column = "tms__id_",
                   std::string init_column = "tms__id_"
                   ) 
      throw(ModelBackendException);  
 private:
  void OpenSession();
  void CloseSession();
  SOCIDBScheme scheme_;
  std::string table_name_;
  bool thread_safe_;  
  soci::session session_;
  std::string id_column_;
  std::string init_column_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SOCI_MODEL_BACKEND_HPP_
