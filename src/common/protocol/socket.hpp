#ifndef _TMS_COMMON_PROTOCOL__SOCKET_HPP_
#define _TMS_COMMON_PROTOCOL__SOCKET_HPP_
// boost
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketP;
}
}
}
#endif // _TMS_COMMON_PROTOCOL__SOCKET_HPP_
