#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <string>
#include <typeinfo>
// boost
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/field_fwd.hpp>
#include <contraption/field_exception.hpp>
#include <contraption/model_backend_exception.hpp>
#include <contraption/contraption_accessor_fwd.hpp>
#include "contraption_fwd.hpp"

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Contraption class. This is an abstract class. Contraption
// represents an object from real world - model instances.
//------------------------------------------------------------
class Contraption {
 public:
  FieldType* GetFieldValue(FieldID field_id)
      throw(FieldException, ModelBackendException);
  
  FieldType* GetFieldValue(const std::string &field)
      throw(FieldException, ModelBackendException);
  
  // Field setters only change contraption object.
  // To save changes there is a Save() method.  
  void SetFieldValue(FieldID field_id, const FieldType* value)
      throw(FieldException);

  void SetFieldValue(const std::string &field, const FieldType* value)
      throw(FieldException);

  void SetFieldValue(FieldID field_id, const FieldType& value)
      throw(FieldException);

  void SetFieldValue(const std::string &field, const FieldType& value)
      throw(FieldException);  
  
  size_t GetFieldNumber() const
      throw();

  FieldID GetFieldID(const std::string &field) const
      throw(FieldException);

  bool IsFieldPrivate(const std::string &field) const
      throw(FieldException);
  
  bool IsFieldPrivate(FieldID field_id) const
      throw(FieldException);

  std::string GetFieldName(FieldID field_id) const
      throw(FieldException);
  
  void Save()
      throw(ModelBackendException);
  void Refresh()
      throw(ModelBackendException);
  
  void Swap(Contraption &other)
      throw();
  Contraption& operator=(const Contraption &other)
      throw();
  Contraption(const Contraption &other)
      throw();
  Contraption(boost::shared_ptr<const Model> model)
      throw();

  static const ContraptionID kNewID; // for Model class
  
  friend class ContraptionAccessor;
 private:
  boost::shared_ptr<const Model> model_;
  std::vector< boost::shared_ptr<FieldType> > values_;
  ContraptionID id_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
