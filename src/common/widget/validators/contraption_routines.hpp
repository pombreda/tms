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


template<class T>
class DefaultGetter {
 public:
  DefaultGetter(boost::function<T()> getter, boost::function<T()> def) :
      getter_(getter),
      def_(def) {    
  }
  T operator()() {
    T val = getter_();
    if (val == T()) {
      return def_();
    } else {
      return val;
    }
  }
 private:
  boost::function<T()> getter_;
  boost::function<T()> def_;
};


template<class T>
class VarRefGetter {
 public:
  VarRefGetter(const T &val) :
      val_(val) {
  }
  T operator()() {
    return val_;
  }
 private:
  const T &val_;
};

template<class T>
class ContraptionSetter {
 public:
  ContraptionSetter(contraption::ContraptionP &ptr, std::string field) :
      ptr_(ptr), field_(field) {
  }
  void operator()(T val) {
    wxASSERT(ptr_);
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


class ContraptionSuggestProvider {
 public:
  ContraptionSuggestProvider (contraption::ContraptionArrayP &ptr, std::string field) :
      ptr_(ptr),
      field_(field) {      
  }

  std::vector<std::string> operator()() {
    std::vector<std::string> ret;
    ptr_->Refresh();
    for (size_t i = 0; i < ptr_->size(); ++i) {
      ret.push_back(ptr_->at(i)->Get<std::string>(field_));
    }
    return ret;
  }
 private:
  contraption::ContraptionArrayP &ptr_;
  std::string field_;
};

}
}
}
}
    

#endif // _TMS_COMMON_WIDGET_VALIDATORS__CONTRAPTION_ROUTINES_HPP_
