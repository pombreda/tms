#include "single_client_server.hpp"
using namespace tms::common::protocol;
SingleClientServer::SingleClientServer() :
    Server() {}
SingleClientServer::SingleClientServer(HandlersMapP handlers_map) :
    Server(handlers_map) {}

    
