#ifndef _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
#define _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <common/contraption/model.hpp>
#include <common/contraption/selecto.hpp>
namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Selector class. Selects =)
//------------------------------------------------------------
class Selector {
  virtual vector<Contraption*> Apply(Model *model);
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
