#ifndef _TMS_COMMON_CONTRAPTION__FIELD_TYPE_FWD_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_TYPE_FWD_HPP_
// std
#include <memory>
// boost
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
namespace tms {
namespace common {
namespace contraption {

class FieldType;
template<typename T>
class FieldTypeT;
typedef std::auto_ptr<FieldType> FieldTypeP;
typedef boost::scoped_array<boost::scoped_ptr<FieldType> > FieldTypeArray;
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__FIELD_TYPE_FWD_HPP_
