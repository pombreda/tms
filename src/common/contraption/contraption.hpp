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
#include <contraption/field_type.hpp>

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Contraption class. This is an abstract class. Contraption
// represents an object from real world - model instances.
//------------------------------------------------------------
typedef size_t ContraptionID;

class Contraption {
 public:
  FieldType* GetField(FieldID field_id) const 
      throw(FieldExcepton, ModelBackendException);
  
  FieldType* GetField(const std::string &field) const
      throw(FieldExcepton, ModelBackendException);
  
  // Field setters only change contraption object.
  // To save changes there is a Save() method.
  void SetField(FieldID field_id, const FieldType* value)
      throw(FieldExcepton);

  void SetField(const std::string &field, const FieldType* value)
      throw(FieldExcepton);
  
  size_t GetFieldNumber() const
      throw();

  FieldID GetFieldID(const std::string &field)
      throw();
  std::string GetFieldName(FieldID field_id) const
      throw(FieldExcepton);
  
  void Save() const
      throw(ModelBackendException);
  void Refresh()
      throw(ModelBackendException);
  
  void Swap(Contraption &other)
      throw();
  Contraption& operator=(const Contraption &other)
      throw();
  Contraption(const Contraption &other)
       throw();
  Contraption(shared_ptr<const Model> model)
      throw(ContraptionException);

  static const ContraptionID kNewID; // for Model class
  
  friend class ContraptionAccessor;
 private:
  void InitValues()
      throw();
  std::vector< boost::shared_ptr<FieldType> > values_;
  boost::shared_ptr<const Model> model_;
  ContraptionID id_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
