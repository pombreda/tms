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

class CompareFilter : public Filter {
 public: 
  CompareFilter(FieldID field_id, CompareID type) 
      throw() :
      field_id_(new FieldID(field_id)),
      field_(),
      type_(type) {}
  
  CompareFilter(const std::string &field, CompareID type) 
      throw() :
      field_id_(),
      field_(new std::string(field)),
      type_(type) {}
  
  virtual SelectorP Apply(const Model *model) 
      throw (FieldException) {
    FieldID field_id;
    if (field_id_.get()) {
      field_id = *field_id_;
    } else {
      field_id = model->GetFieldID(*field_);
    }
    return model->GetField(field_id)->GetSelector(this);
  }

  CompareID type() 
      throw() {
    return type_;
  }
      
 private:
  std::auto_ptr<FieldID> field_id_;
  std::auto_ptr<std::string> field_;
  CompareID type_;
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__COMPARE_FILTER_HPP_
