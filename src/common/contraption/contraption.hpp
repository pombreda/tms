#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <string>
#include <sstream>
#include <typeinfo>
// boost
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/field_type_fwd.hpp>
#include <contraption/field_type.hpp>
#include <contraption/field_fwd.hpp>
#include <contraption/field_exception.hpp>
#include <contraption/model_backend_exception.hpp>
#include <contraption/contraption_accessor_fwd.hpp>
//
#include "contraption_fwd.hpp"

//------------------------------------------------------------
// For intrusive_ptr.
//------------------------------------------------------------
namespace boost {
inline void intrusive_ptr_add_ref(tms::common::contraption::Contraption* contraption);
inline void intrusive_ptr_release(tms::common::contraption::Contraption* contraption);
}

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Contraption class. This is an abstract class. Contraption
// represents an object from real world - model instances.
//------------------------------------------------------------
class Contraption {
 public:
  // It is strongly recommended to rok with field_id version
  // as its performance is much greater
  template<typename T>
  T Get(FieldID field_id)
      throw(FieldException, ModelBackendException);
  template<typename T>
  T Get(const std::string &field)
      throw(FieldException, ModelBackendException);

  // Field setters only change contraption object.
  // To save changes there is a Save() method.  

  template<typename T>
  void Set(FieldID field_id, const T &value)
      throw(FieldException, ModelBackendException);
  template<typename T>
  void Set(const std::string &field, const T &value)
      throw(FieldException, ModelBackendException);

  FieldID GetID(const std::string &field) const
      throw(FieldException);
  std::string GetName(FieldID field_id) const
      throw(FieldException);
 
  size_t GetFieldNumber() const
      throw();

  bool IsPrivate(const std::string &field) const
      throw(FieldException);
  bool IsPrivate(FieldID field_id) const
      throw(FieldException);
  
  void Save()
      throw(ModelBackendException);
  void Delete()
      throw(ModelBackendException);
  void Refresh()
      throw();
  
  // Copying will clone contraption and next save will create
  // a sepparate record for it.
  Contraption& operator=(const Contraption &other)
      throw();
  // Copying will clone contraption and next save will create
  // a sepparate record for it.
  Contraption(const Contraption &other)
      throw();
  Contraption(ModelP model)
      throw();

  void Swap(Contraption &other)
      throw();
  
  //------------------------------------------------------------
  // Implementation. Do not use this method
  //------------------------------------------------------------
  FieldType* GetFieldValue(FieldID field_id)
      throw(FieldException, ModelBackendException);
  
  void SetFieldValue(FieldID field_id, const FieldType& value)
      throw(FieldException);

  static const ContraptionID kNewID; // for Model class
  
  friend class ContraptionAccessor;
  friend class ContraptionArray;
  template<typename T>
  friend class SimpleFieldT;
  friend void boost::intrusive_ptr_add_ref(Contraption* contraption);
  friend void boost::intrusive_ptr_release(Contraption* contraption);
 private:
  size_t ptr_count_;
  bool in_array_;
  boost::signal<void ()>  on_change_;
  ModelP model_;
  boost::scoped_array<boost::scoped_ptr<FieldType> > values_;
  ContraptionID id_;
};

template<typename T>
T Contraption::Get(FieldID field_id)
    throw(FieldException, ModelBackendException) {
  FieldTypeT<T> *ptr = dynamic_cast<FieldTypeT<T>*>(
      GetFieldValue(field_id));
  if (ptr) {
    return ptr->data();
  } else {
    std::ostringstream msg;
    msg << "Incorrect field type in Contraption::Get - field: '"
        << GetName(field_id) << "' of type: '"
        << typeid(model_->GetField(field_id)).name()
        << "' can not be interpreted as value of type '"
        << typeid(T).name() << "'.";
    throw FieldException(msg.str());
  }
}

template<typename T>
T Contraption::Get(const std::string &field)
    throw(FieldException, ModelBackendException) {
  return Get<T>(GetID(field));
}

template<typename T>
void Contraption::Set(FieldID field_id, const T &value)
    throw(FieldException, ModelBackendException) {
  SetFieldValue(field_id, FieldTypeT<T>(value));
}

template<typename T>
void Contraption::Set(const std::string &field, const T &value)
    throw(FieldException, ModelBackendException) {
  Set(GetID(field), value);
}



}
}
}

namespace boost {
inline void intrusive_ptr_add_ref(tms::common::contraption::Contraption* contraption) {
  ++contraption->ptr_count_;
}

inline void intrusive_ptr_release(tms::common::contraption::Contraption* contraption) {
  if(!(--contraption->ptr_count_))
    delete contraption;
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
