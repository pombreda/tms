#ifndef _TMS_COMMON_CONTRAPTION__SOCI_MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__SOCI_MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// boost
#include <boost/shared_ptr.hpp>
// soci
#include <soci.h>
// common
#include <contraption/model_backend.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/model_fwd.hpp>
#include <contraption/contraption_fwd.hpp>


namespace tms {
namespace common {
namespace contraption {

struct SOCIDBScheme {
  const soci::backend_factory &factory;
  const std::string connection_string;
  SOCIDBScheme(const soci::backend_factory &factory, 
               const std::string &connection_string) 
      throw():
      factory(factory),
      connection_string(connection_string) {}
};

typedef boost::shared_ptr<SOCIDBScheme> SOCIDBSchemeP;
//------------------------------------------------------------
// SOCIModelBackend class.
// It is not thread-safe. Moreover It'll breake if any
// other process will write to the db.
// While it is possible to rewrite it to be thread-safe
// it'll require special functions of concrete sql realization
// like sqlite3_last_insert_rowid.
//------------------------------------------------------------
class SOCIModelBackend : public ModelBackend {
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

typedef boost::shared_ptr<SOCIModelBackend> SOCIModelBackendP;

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SOCI_MODEL_BACKEND_HPP_
