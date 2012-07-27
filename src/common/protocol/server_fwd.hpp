#ifndef _TMS_COMMON_PROTOCOL__SERVER_FWD_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_FWD_HPP_

#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {

class Server;

typedef boost::shared_ptr<Server> ServerP;

}
}
}  
#endif // _TMS_COMMON_PROTOCOL__SERVER_FWD_HPP_
