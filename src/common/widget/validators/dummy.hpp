#ifndef _TMS_COMMON_WIDGET_VALIDATORS__DUMMY_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__DUMMY_HPP_
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
class DummyGetter {
 public:
  T operator()() {
    return T();
  }
};

template<class T>
class DummySetter {
 public:
  void operator()(T /*val*/) {
    return;
  }
};

}
}
}
}
    

#endif // _TMS_COMMON_WIDGET_VALIDATORS__DUMMY_HPP_
