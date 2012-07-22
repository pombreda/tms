#ifndef _TMS_COMMON_PROTOCOL__STREAM_HPP_
#define _TMS_COMMON_PROTOCOL__STREAM_HPP_
// std
#include <istream>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
typedef boost::shared_ptr<std::iostream> StreamP;
}
}
}
#endif // _TMS_COMMON_PROTOCOL__STREAM_HPP_
