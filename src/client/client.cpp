#include "client.hpp"
// common
#include <project/protocol.hpp>
#include <common/protocol/socket_client.hpp>
using namespace tms::common::protocol;
using namespace tms::project;
using namespace std;
namespace tms {
namespace client {
ClientP CreateClient(const string &server, const string &port) {
   return ClientP(new SocketClient(server, port, 
                                   AppProtocol())); 

}
}
}
