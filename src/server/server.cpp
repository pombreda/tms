#include "server.hpp"
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <protocol/tcp_server.hpp>
#include <protocol/crypto.hpp>
#include <protocol/model_backend_request_processor.hpp>
#include <protocol/login_request_processor.hpp>
#include <protocol/simple_request_processor.hpp>
#include <model/user.hpp>
#include <project/model/contact_person.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <project/protocol.hpp>
using namespace std;
using namespace tms::server;
using namespace tms::common::protocol;
using namespace tms::common::contraption;
using namespace tms::project;
using namespace tms::project::model;
using namespace tms::common::model;
namespace tms {
namespace server {

ServerP CreateServer(const std::string &port, const std::string &db) {
  // Create RequestProcessor
  SOCIDBScheme scheme(soci::sqlite3, db);
  ModelP users(User::GetModel(ModelBackendP(new SOCIModelBackend(scheme, "users"))));
  SimpleRequestProcessorP request_processor(new SimpleRequestProcessor());
  ModelBackendRequestProcessor::Register(*request_processor, scheme);
  RequestProcessorP processor(
      new LoginRequestProcessor(request_processor, users));
  // Create Server
  return ServerP(
      new TCPServer(port,
                    AppProtocol(),
                    processor));

}

void InitSchema(const std::string &db) {
  SOCIDBScheme scheme(soci::sqlite3, db);
  {
    ModelP users = User::GetModel(ModelBackendP(new SOCIModelBackend(scheme, "users")));
    users->InitSchema();
    ContraptionP admin = users->New();
    admin->Set<string>("name", "admin");
    admin->Set<string>("password_hash", sha256("admin"));
    admin->Save();
  }
  {
    ModelP contact_persons =
        ContactPerson::GetModel(ModelBackendP(new SOCIModelBackend(scheme,
                                                             "contact_persons")));
    contact_persons->InitSchema();
  }
}
}
}
