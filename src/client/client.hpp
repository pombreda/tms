#ifndef _TMS_CLIENT__CLIENT_HPP_
#define _TMS_CLIENT__CLIENT_HPP_
// std
#include <string>
#include <protocol/client.hpp>
namespace tms {
namespace client {
common::protocol::ClientP CreateClient(const std::string &server, 
                                       const std::string &port);
}
}
#endif // _TMS_CLIENT__CLIENT_HPP_
