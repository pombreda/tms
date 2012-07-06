#ifndef _TMS_COMMON_CONTRAPTION__FIELD_TYPE_FWD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_TYPE_FWD_HPP_
#include <memory>
namespace tms {
namespace common {
namespace contraption {

class FieldType;
template<typename T>
class FieldTypeT;
typedef std::auto_ptr<FieldType> FieldTypeP;
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_TYPE_FWD_HPP_
