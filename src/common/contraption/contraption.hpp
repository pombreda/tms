#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <common/contraption/model.hpp>
#include <typeinfo>

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Contraption class. This is an abstract class. Contraption
// represents an object from real world.
//------------------------------------------------------------
typedef size_t ContraptionID; //private?

class Contraption {
 public:
  // GetField can throw a runtime exception if out type does not
  // correspond to field type. It'll be greate to find a way
  // to throw static_exception here, but this looks 
  // impossible.
  template<class T>
  void GetField(FieldID field_id, T &out) const;

  template<class T>
  void GetField(const std::string &field, T &out) const;

  // SetField can throw a runtime exception if value type does not
  // correspond to field type. It'll be greate to find a way
  // to throw static_exception here, but this looks 
  // impossible.
  template<class T>
  void SetField(FieldID field_id, const T &value);

  template<class T>
  void SetField(const std::string &field, const T &value);

  FieldType* GetType(FieldID field_id);
  FieldType* GetType(const std::string &field);

  size_t GetFieldNumber();

  FieldID GetFieldID(const std::string &field);
  std::string GetFieldName(FieldID field_id);
  
  void Save();
  void Refresh();

  Contraption(const Contraption &other);
  Contraption(const Model* model);
  Contraption(const Model* model, ContraptionID id);
  Contraption(); //for serialization

  virtual ~Contraption();
  
  static const ContraptionID kNewID;

  ContraptionID id() {
    return id_;
  }

  Model* model() {
    return model_;
  }

 private:
  void FreeValues();
  void InitValues();
  // ToDo union for built_int types
  vector<void*> values_;
  Model *model_;
  ContraptionID id_; //zero is a special value
};

#include "contraption_impl.hpp"
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_HPP_
