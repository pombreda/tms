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
inline void intrusive_ptr_add_ref(tms::common::contraption::Contraption *contraption);
inline void intrusive_ptr_release(tms::common::contraption::Contraption *contraption);
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
  // It is strongly recommended to work with field_id version
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

  bool IsReadable(const std::string &field) const
      throw(FieldException);
  bool IsReadable(FieldID field_id) const
      throw(FieldException);
  bool IsWritable(const std::string &field) const
      throw(FieldException);
  bool IsWritable(FieldID field_id) const
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

  void Swap(Contraption &other)
      throw();
  
  ModelP model() { return model_; }

  static const ContraptionID kNewID; // for Model class

  friend class ContraptionAccessor;
  friend class ContraptionArray;
  friend class Model;
  friend void boost::intrusive_ptr_add_ref(Contraption *contraption);
  friend void boost::intrusive_ptr_release(Contraption *contraption);

  FieldTypeP GetFieldValue(FieldID field_id)
      throw(FieldException, ModelBackendException);

  void SetFieldValue(FieldID field_id, const FieldType& value)
      throw(FieldException);

 private:
  Contraption(ModelP model)
      throw();
  size_t ptr_count_;
  bool in_array_;
  boost::signal<void ()> on_change_;
  boost::signal<void ()> on_delete_;
  ModelP model_;
  FieldTypeArray values_;
  ContraptionID id_;
  bool saving_;
};
}
}
}
#include "contraption_impl.hpp"
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
