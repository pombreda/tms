#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_FWD_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_FWD_HPP_
#include <boost/intrusive_ptr.hpp>
namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Contraption class. This is an abstract class. Contraption
// represents an object from real world - model instances.
//------------------------------------------------------------
typedef size_t ContraptionID;
class Contraption;
typedef boost::intrusive_ptr<Contraption> ContraptionP;
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_FWD_HPP_
