#ifndef _TMS_COMMON_CONTRAPTION__RECORD_FWD_HPP_
#define _TMS_COMMON_CONTRAPTION__RECORD_FWD_HPP_
// std
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace contraption {
class Record;

typedef boost::shared_ptr<Record> RecordP;

template<typename T>
class RecordT;

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__RECORD_FWD_HPP_
