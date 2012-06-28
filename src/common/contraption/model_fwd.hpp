#ifndef _TMS_COMMON_CONTRAPTION__MODEL_FWD_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_FWD_HPP_
#include <boost/intrusive_ptr.hpp>
namespace tms {
namespace common {
namespace contraption {

typedef size_t FieldID;

class Model;
typedef boost::intrusive_ptr<Model> ModelP;
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_FWD_HPP_
