#ifndef _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
#define _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <memory>
#include <string>
// boost 
#include <boost/lexical_cast.hpp>
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

  CompareFilterT(const std::string &backend_name, CompareID type, const T& value) 
      throw() :
      backend_name_(backend_name),
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

  std::string ToString() const {
    std::string ret = backend_name_ + " ";
    switch (type_) {
      case kEqual:
        ret += "==";
        break;
      case kNotEqual:
        ret += "!=";
        break;
      case kLesser:
        ret += "<";
        break;
      case kNotLesser:
        ret += ">=";
        break;
      case kGreater:
        ret += ">";
        break;
      case kNotGreater:
        ret += "<=";
        break;
    }
    ret += " ";
    ret += boost::lexical_cast<std::string>(value_);
    return ret;
  }
 private:
  const std::string &backend_name_; 
  CompareID type_;
  const T value_;
};

template <class T>
FilterP Compare (const SimpleFieldT<T> *field, CompareID type, const T &value) {
  return FilterP(new CompareFilterT<T>(field, type, value));
}

inline FilterP Compare (const std::string &backend_name, 
                        CompareID type, const int &value) {
  return FilterP(new CompareFilterT<int>(backend_name, type, value));
}

inline FilterP Compare (const std::string &backend_name, 
                        CompareID type, const std::string &value) {
  return FilterP(new CompareFilterT<std::string>(backend_name, type, value));
}


}
}
}
#endif // _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
