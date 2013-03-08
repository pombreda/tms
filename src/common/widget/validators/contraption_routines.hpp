#ifndef _TMS_COMMON_WIDGET_VALIDATORS__CONTRAPTION_ROUTINES_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__CONTRAPTION_ROUTINES_HPP_
// std
#include <string>
// boost
#include <boost/function.hpp>
// wx
#include <wx/valtext.h>
// common
#include <contraption/contraption.hpp>

namespace tms {
namespace common {
namespace widget {
namespace validators {

template<class T>
class ContraptionGetter {
 public:
  ContraptionGetter(tms::common::contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  T operator()() {
    return ptr_->Get<T>(field_);
  }
 private:
  tms::common::contraption::ContraptionP &ptr_;
  std::string field_;
};

template<>
class ContraptionGetter<bool> {
 public:
  ContraptionGetter(tms::common::contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  bool operator()() {
    return ptr_->Get<int>(field_);
  }
 private:
  tms::common::contraption::ContraptionP &ptr_;
  std::string field_;
};

template<class T>
class ContraptionSetter {
 public:
  ContraptionSetter(tms::common::contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  void operator()(T val) {
    ptr_->Set<T>(field_, val);
  }
 private:
  tms::common::contraption::ContraptionP &ptr_;
  std::string field_;  
};

template<>
class ContraptionSetter<bool> {
 public:
  ContraptionSetter(tms::common::contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  void operator()(bool val) {
    ptr_->Set<int>(field_, val);
  }
 private:
  tms::common::contraption::ContraptionP &ptr_;
  std::string field_;  
};

class ContraptionIsNew {
 public:
  ContraptionIsNew(tms::common::contraption::ContraptionP &ptr) :
      ptr_(ptr) {
  }
  bool operator()() {
    return ptr_->IsNew();
  }
 private:
  tms::common::contraption::ContraptionP &ptr_;
};


}
}
}
}
    

#endif // _TMS_COMMON_WIDGET_VALIDATORS__CONTRAPTION_ROUTINES_HPP_
