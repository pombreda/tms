#ifndef _TMS_SERVER__SERVER_HPP_
#define _TMS_SERVER__SERVER_HPP_
// std
#include <string>
// common
#include <common/protocol/server.hpp>
namespace tms {
namespace server {
common::protocol::ServerP CreateServer(const std::string &port, 
                                       const std::string &db);
void InitSchema(const std::string &db);
}
}
#endif // _TMS_SERVER__SERVER_HPP_
