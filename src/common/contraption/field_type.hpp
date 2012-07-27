#ifndef _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include "field_type_fwd.hpp"
// standart
#include <cstdlib>
// boost
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/lexical_cast.hpp>
// 
#include <contraption/field_fwd.hpp>
#include <contraption/field/simple_field_fwd.hpp>
#include <rtti/typeinfo.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// FieldType interface.
//------------------------------------------------------------

class FieldType {
 public:
  virtual ~FieldType() {}
  virtual FieldType* Duplicate() const = 0;
  virtual inline std::string ToString() const {return rtti::TypeID(*this).name();};
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
  T& data() { return data_; }
  void set_data(const T & data) { data_ = data; }
  virtual FieldTypeT<T>* Duplicate() const { return new FieldTypeT<T>(data_); } 
  inline std::string ToString() const;
  friend class SimpleFieldTImpl<T>;
 private:
  T data_;
};

template<class T>
inline std::string FieldTypeT<T>::ToString() const {
  return rtti::TypeID(*this).name();;
}

template<>
inline std::string FieldTypeT<std::string>::ToString() const {
  return data_;
}

template<>
inline std::string FieldTypeT<int>::ToString() const {
  return boost::lexical_cast<std::string>(data_);
}


}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
