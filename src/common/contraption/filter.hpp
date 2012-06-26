#ifndef _TMS_COMMON_CONTRAPTION__FILTER_HPP_
#define _TMS_COMMON_CONTRAPTION__FILTER_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <contraption/model.hpp>
#include <contraption/selector.hpp>
namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Filter class. Applying to model will give a Selector for
// model filtering.
//------------------------------------------------------------
class Filter {
 public:
  virtual Selector* Apply(Model *model);
  virtual ~Filter() {}
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FILTER_HPP_
