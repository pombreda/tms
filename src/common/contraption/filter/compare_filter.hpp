#ifndef _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
#define _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <memory>
#include <string>
// common
#include <contraption/filter.hpp>
#include <contraption/field_exception.hpp>
#include <contraption/compare.hpp>

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// CompareFilter class.
//------------------------------------------------------------

template <class T>
class CompareFilterT : public Filter {
 public: 
  CompareFilterT(const SimpleFieldT<T> *field, CompareID type, const T& value) 
      throw() :
      backend_name_(field->backend_name()),
      type_(type),
      value_(value) {}
         
  CompareID type() const
      throw() {
    return type_;
  }

  const std::string &backend_name() const {
    return backend_name_;
  }

  const T &value() const {
    return value_;
  }
      
 private:
  const std::string &backend_name_; 
  CompareID type_;
  const T value_;
};

template <class T>
FilterP Compare (const SimpleFieldT<T> *field, CompareID type, const T& value) {
  return FilterP(new CompareFilterT<T>(field, type, value));
}

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
