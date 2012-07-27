// std
#include <string>
// boost
#include <boost/asio.hpp>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// log4cplus
#include <log4cplus/configurator.h>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/server_model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/field_type.hpp>
#include <model/user.hpp>
#include <protocol/model_backend_protocol.hpp>
#include <protocol/tcp_server.hpp>
#include <protocol/stream_client.hpp>
#include <protocol/message.hpp>
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <protocol/message/error_response.hpp>
#include <protocol/simple_request_processor.hpp>
#include <protocol/model_backend_request_processor.hpp>
#include <protocol/model_backend_protocol.hpp>

using namespace std;
using namespace tms::common::contraption;
using namespace tms::common::protocol;
using namespace tms::common::model;
using boost::asio::ip::tcp;
using namespace log4cplus;


int main () {
  // Configuse logger
  PropertyConfigurator config("log.cfg");
  config.configure();
  // DB data
  string test_db("test.sqlite3");
  remove(test_db.c_str());
  SOCIDBScheme scheme(soci::sqlite3, test_db);    
  // InitSchema
  ModelP users(new User(ModelBackendP(new SOCIModelBackend(scheme, "users"))));
  users->InitSchema();
  users.reset();
  vector<Field*> fields;
  fields.push_back(new SimpleFieldT<string>("name"));
  fields.push_back(new SimpleFieldT<int>("age"));
  fields.push_back(new SimpleFieldT<int>("password", 
                                         _is_readable = false));
  fields.push_back(new SimpleFieldT<string>("Surname", 
                                            _backend_name = "surname"));
  ModelP soci_model(new Model(fields, new SOCIModelBackend(scheme, "test")));    
  soci_model->InitSchema();
  soci_model.reset();
  // Init Models
  users.reset(new User(ModelBackendP(new SOCIModelBackend(scheme, "users"))));
  fields.clear();
  fields.push_back(new SimpleFieldT<string>("name"));
  fields.push_back(new SimpleFieldT<int>("age"));
  fields.push_back(new SimpleFieldT<int>("password", 
                                           _is_readable = false));
  fields.push_back(new SimpleFieldT<string>("Surname", 
                                            _backend_name = "surname"));
  soci_model.reset(new Model(fields, new SOCIModelBackend(scheme, "test")));    
  // user added
  ContraptionP user = users->New();
  user->Set<string>("name", "adavydow");
  user->Set<string>("password_hash", "Dummy");
  user->Save();
    // Create Protocol
  ModelBackendProtocolP protocol(new ModelBackendProtocol());
  protocol->Initialize();
  // Create RequestProcessor
  SimpleRequestProcessorP request_processor(new SimpleRequestProcessor());
  RequestProcessorP processor(
      new ModelBackendRequestProcessor(request_processor, users, scheme));
  // Create Server
  ServerP server(
      new TCPServer(tcp::endpoint(tcp::v4(), 3030), 
                      protocol, 
                    processor));  
  server->Listen();  
  server->Wait();
  return 0;
}
