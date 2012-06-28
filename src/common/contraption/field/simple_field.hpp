#ifndef _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <contraption/field.hpp>
#include <contraption/record.hpp>
#include <contraption/contraption.hpp>
namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// SimpleField class. Represents a simple mapping to db.
// This is an abstract class. SimpleFieldT - concrete 
// realization.
//------------------------------------------------------------
class SimpleField : virtual public Field {  
 public:
  SimpleField(const std::string &name, bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_), 
      backend_name_(name) {}

  SimpleField(const std::string &name, const std::string &backend_name_,
         bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_), 
      backend_name_(backend_name_) {}
  virtual ~SimpleField() {}
 protected:
  const std::string backend_name_;  
};

//------------------------------------------------------------
// SimpleFieldT class. Represents a simple mapping to db.
//------------------------------------------------------------
template<class T>
class SimpleFieldT : virtual public SimpleField,
                     virtual public FieldT<T> {  
 public:
  SimpleFieldT(const std::string &name, bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_),
      SimpleField(name, is_private_),
      FieldT<T>(name, is_private_),
      field_id_(9999) {}

  SimpleFieldT(const std::string &name, const std::string &backend_name_,
         bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_),
      SimpleField(name, backend_name_, is_private_),
      FieldT<T>(name, is_private_),
      field_id_(9999) {}

  SimpleFieldT(const std::string &name, const char *backend_name_,
         bool is_private_ = false)
      throw(FieldException) : 
      Field(name, is_private_),
      SimpleField(name, backend_name_, is_private_),
      FieldT<T>(name, is_private_),
      field_id_(9999) {}

  virtual void Initialize(Model *model) {
    field_id_ = model->GetFieldID(name());
  }
  
  virtual void GetReadRecords(Contraption *contraption, 
                          std::vector<Record*> &out) const {
    if (!contraption->values_[(int)field_id_]) {
      contraption->values_[(int)field_id_].reset(new FieldTypeT<T>());
      if (contraption->id_ != Contraption::kNewID) {
        out.push_back(new RecordT<T>(backend_name_, 
                                     &(dynamic_cast<FieldTypeT<T>*>(
                                         contraption->values_[(int)field_id_].get())->data_)));
      }
    }
  }
  
  virtual void GetWriteRecords(Contraption *contraption, 
                          std::vector<Record*> &out) const {
    if (!contraption->values_[(int)field_id_] 
        && contraption->id_ == Contraption::kNewID) {
      contraption->values_[(int)field_id_].reset(new FieldTypeT<T>());
    }
    if (contraption->values_[(int)field_id_]) {
      out.push_back(new RecordT<T>(backend_name_, 
                                   &(dynamic_cast<FieldTypeT<T>*>(
                                       contraption->values_[(int)field_id_].get())->data_)));
    }
  }

  virtual ~SimpleFieldT() {}
 private:
  FieldID field_id_;
};


}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SIMPLE_FIELD_HPP_
