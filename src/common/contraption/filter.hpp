#ifndef _TMS_COMMON_CONTRAPTION__FILTER_HPP_
#define _TMS_COMMON_CONTRAPTION__FILTER_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <common/contraption/model.hpp>
#include <common/contraption/selecto.hpp>
namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Filter class. Applying to model will give a Selector for
// model filtering.
//------------------------------------------------------------
class Filter {
  virtual Selector* Apply(Model *model);
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FILTER_HPP_
