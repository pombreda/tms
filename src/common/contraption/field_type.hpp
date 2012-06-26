#ifndef _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <cstdlib>
namespace tms {
namespace common {
namespace contraption {

typedef size_t FieldTypeID;
// Specialize this function for your type
template<class T>
FieldTypeID GetFieldTypeID();

//------------------------------------------------------------
// FieldType interface.
//------------------------------------------------------------

class FieldType {
 public:
  virtual ~FieldType(){}
  FieldType* Duplicate() = 0;
};
                                               
//------------------------------------------------------------
// FieldTypeT<T> class. Wrapper for a c++ type.
//------------------------------------------------------------
template<typename T>
class FieldTypeT : FieldType {
  BOOST_CONCEPT_ASSERT((Assignable<T>));
  BOOST_CONCEPT_ASSERT((DefaultConstructible<T>));
  BOOST_CONCEPT_ASSERT((CopyConstructible<T>));
 public:
  FieldTypeT() : data_() {}
  FieldTypeT(T data) : data_(data) {}
  virtual ~FieldTypeT(){}
  T data() const { return data_; }
  void set_data(const T & data) {data_ = data;}
  FieldTypeT<T>* Duplicate() const { return new FieldTypeT<T>(data_) } ;
 private:
  T data_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
