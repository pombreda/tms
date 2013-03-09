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
  ContraptionGetter(contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  T operator()() {
    return ptr_->Get<T>(field_);
  }
 private:
  contraption::ContraptionP &ptr_;
  std::string field_;
};

template<>
class ContraptionGetter<bool> {
 public:
  ContraptionGetter(contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  bool operator()() {
    return ptr_->Get<int>(field_);
  }
 private:
  contraption::ContraptionP &ptr_;
  std::string field_;
};

template<>
class ContraptionGetter<contraption::ContraptionP> {
 public:
  ContraptionGetter(contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  contraption::ContraptionP operator()() {
    contraption::ContraptionArrayP array =
        ptr_->Get<contraption::ContraptionArrayP>(field_);
    if (array->size() > 0) {
    }
  }
 private:
  contraption::ContraptionP &ptr_;
  std::string field_;
};


template<class T>
class ContraptionSetter {
 public:
  ContraptionSetter(contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  void operator()(T val) {
    ptr_->Set<T>(field_, val);
  }
 private:
  contraption::ContraptionP &ptr_;
  std::string field_;  
};

template<>
class ContraptionSetter<bool> {
 public:
  ContraptionSetter(contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  void operator()(bool val) {
    ptr_->Set<int>(field_, val);
  }
 private:
  contraption::ContraptionP &ptr_;
  std::string field_;  
};

class ContraptionIsNew {
 public:
  ContraptionIsNew(contraption::ContraptionP &ptr) :
      ptr_(ptr) {
  }
  bool operator()() {
    return ptr_->IsNew();
  }
 private:
  contraption::ContraptionP &ptr_;
};


}
}
}
}
    

#endif // _TMS_COMMON_WIDGET_VALIDATORS__CONTRAPTION_ROUTINES_HPP_
