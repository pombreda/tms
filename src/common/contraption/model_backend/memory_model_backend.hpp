#ifndef _TMS_COMMON_CONTRAPTION__MEMORY_MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__MEMORY_MODEL_BACKEND_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <map>
// common
#include <contraption/model_backend.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/model_fwd.hpp>
#include <contraption/contraption_fwd.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// MemoryModelBackend class. This class is mainly for
// unit testing.
//------------------------------------------------------------
class MemoryModelBackend : public ModelBackend {
 public:
  virtual FieldType* GetField(FieldID field_id, 
                              Contraption *contraption)
      throw(ModelBackendException);      
  virtual void Save(Contraption *contraption)
      throw(ModelBackendException);
  std::map<ContraptionID, std::map<std::string, int> >& int_fields()
      throw() {return int_fields_;}
  std::map<ContraptionID, std::map<std::string, std::string> >& string_fields()
      throw() {return string_fields_;}
  MemoryModelBackend() : 
      int_fields_(),
      string_fields_() {}
 private:
  std::map<ContraptionID, std::map<std::string, int> > int_fields_;
  std::map<ContraptionID, std::map<std::string, std::string> > string_fields_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MEMORY_MODEL_BACKEND_HPP_
