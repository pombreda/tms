// std
#include <cstdio>
// boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include <iostream>//oops
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
#include <contraption/contraption_accessor.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <protocol/model_backend_protocol.hpp>
#include <protocol/tcp_server.hpp>
#include <protocol/socket_client.hpp>
#include <protocol/message.hpp>
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <protocol/message/error_response.hpp>
#include <protocol/simple_request_processor.hpp>
#include <protocol/model_backend_request_processor.hpp>
#include <protocol/model_backend_protocol.hpp>
#include <model/user.hpp>

using namespace std;
using namespace tms::common::contraption;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;
using namespace tms::common::model;
using boost::asio::ip::tcp;
using namespace log4cplus;


int main() {
  // Configuse logger
  PropertyConfigurator config("log.cfg");
  config.configure();
  ModelBackendProtocolP protocol(new ModelBackendProtocol());
  protocol->Initialize();
  ClientP client(
      new SocketClient("localhost", "3030", protocol)); 
  LoginRequestP login(new LoginRequest);
  login->set_name("adavydow");
  login->set_password_hash("Dummy");


  for (int i = 0; i < 100000; ++i) {
    MessageP ret = client->EvalRequest(*login);  
  }
  return 0;
}
