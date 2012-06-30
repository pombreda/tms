#ifndef _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
#define _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// boost
#include <boost/shared_ptr.hpp>

namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// Selector class. Selects =)
//------------------------------------------------------------
class Selector {
 public:
  virtual ~Selector() {}
};

typedef boost::shared_ptr<Selector> SelectorP;
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__SELECTOR_HPP_
