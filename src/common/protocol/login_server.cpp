#include "login_server.hpp"
using namespace tms::common::protocol;
using namespace tms::common::contraption;
using namespace std;
LoginServer::LoginServer(ServerP server, ModelP users) 
    throw():
    DecoratorServer(server),
    users_(users),
    user_ {}



void DecoratorServer::AddHandler(boost::function<MessageP (const Message&)> handler, 
                                 const rtti::TypeInfo &typeinfo)
    throw() {
  server_->AddHandler(handler, typeinfo);
}
