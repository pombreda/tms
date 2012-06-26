#ifndef _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// standart
#include <cstdlib>
// boost
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
// 
#include "field_type_fwd.hpp"

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// FieldType interface.
//------------------------------------------------------------

class FieldType {
 public:
  virtual ~FieldType(){}
  virtual FieldType* Duplicate() const = 0;
};
                                               
//------------------------------------------------------------
// FieldTypeT<T> class. Wrapper for a c++ type.
//------------------------------------------------------------
template<typename T>
class FieldTypeT : public FieldType {
  BOOST_CONCEPT_ASSERT((boost::Assignable<T>));
  BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<T>));
  BOOST_CONCEPT_ASSERT((boost::CopyConstructible<T>));
 public:
  FieldTypeT() : data_() {}
  FieldTypeT(T data) : data_(data) {}
  virtual ~FieldTypeT(){}
  T data() const { return data_; }
  void set_data(const T & data) {data_ = data;}
  virtual FieldTypeT<T>* Duplicate() const { return new FieldTypeT<T>(data_); } 
 private:
  T data_;
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
