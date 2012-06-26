#ifndef _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
#define _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <vector>
#include <contraption/model_fwd.hpp>
#include <contraption/contraption_fwd.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Selector class. Selects =)
//------------------------------------------------------------
class Selector {
 public:
  virtual std::vector<Contraption*> Apply(Model *model);
  virtual ~Selector() {}
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
