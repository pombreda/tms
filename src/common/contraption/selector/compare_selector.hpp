#ifndef _TMS_COMMON_CONTRAPTION__COMPARE_SELECTOR_HPP_
#define _TMS_COMMON_CONTRAPTION__COMPARE_SELECTOR_HPP_
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

class CompareSelector : public Selector {
 public: 
  CompareSelector(const std::string &field, CompareID type) 
      throw() :
      field_(field),
      type_(type) {}
  
  CompareID type() 
      throw() {
    return type_;
  }

  const std::string& field() 
      throw() {
    return field_;
  }
  
      
 private:
  const std::string& field_;
  CompareID type_;
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__COMPARE_SELECTOR_HPP_
