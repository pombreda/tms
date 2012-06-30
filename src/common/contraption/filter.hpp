#ifndef _TMS_COMMON_CONTRAPTION__FILTER_HPP_
#define _TMS_COMMON_CONTRAPTION__FILTER_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// boost
#include <boost/shared_ptr.hpp>
// common
#include <contraption/model_fwd.hpp>
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
  virtual SelectorP Apply(Model *model);
  virtual ~Filter() {}
};

typedef boost::shared_ptr<Filter> FilterP;

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FILTER_HPP_
