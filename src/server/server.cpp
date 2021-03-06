#include "server.hpp"
// soci
#include <sqlite3/soci-sqlite3.h>
// common
#include <protocol/tcp_server.hpp>
#include <protocol/crypto.hpp>
#include <protocol/model_backend_request_processor.hpp>
#include <protocol/login_request_processor.hpp>
#include <protocol/simple_request_processor.hpp>
#include <protocol/patcher_request_processor.hpp>
#include <model/user.hpp>
#include <project/model/contact_person.hpp>
#include <project/model/company.hpp>
#include <project/model/scheme.hpp>
#include <project/model/incoming.hpp>
#include <project/model/subject.hpp>
#include <project/model/addressee.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <project/protocol.hpp>
#include <config.hpp>
#include <iostream>
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
  User::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme, "users")));
  ModelP users(User::GetModel());
  SimpleRequestProcessorP request_processor(new SimpleRequestProcessor());
  ModelBackendRequestProcessor::Register(*request_processor, scheme);
  PatcherRequestProcessor::Register(*request_processor);
  RequestProcessorP processor(
      new LoginRequestProcessor(request_processor, users));
  // Create Server
  return ServerP(
      new TCPServer(port,
                    AppProtocol(),
                    processor));

}

void InitSchema(const std::string &db) {
  try {
  {
    SOCIDBScheme scheme(soci::sqlite3, db);
    User::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme, "users")));
    Scheme::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme , "scheme")));
    Incoming::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme, "incomings")));
    ContactPerson::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme,
								   "contact_persons")));
    Company::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme,
                                                             "companies")));
    Subject::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme,
                                                             "subjects")));

    Addressee::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme,
							       "addressees")));

    
    User::GetModel()->InitSchema();

    Scheme::GetModel()->InitSchema();
    
    Incoming::GetModel()->InitSchema();

    ContactPerson::GetModel()->InitSchema();

    Company::GetModel()->InitSchema();

    Subject::GetModel()->InitSchema();

    Addressee::GetModel()->InitSchema();

  }
  {
    SOCIDBScheme scheme(soci::sqlite3, db);
    User::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme, "users")));
    Scheme::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme , "scheme")));
    Incoming::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme, "incomings")));
    ContactPerson::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme,
								   "contact_persons")));
    Company::PrepareModel(ModelBackendP(new SOCIModelBackend(scheme,
							     "companies")));
    ModelP users = User::GetModel();
    
    ModelP scheme_m = Scheme::GetModel();
    
    ModelP incomings = Incoming::GetModel();
    
    ModelP contact_persons =
      ContactPerson::GetModel();
    ContraptionP scheme_i = scheme_m->New();
    scheme_i->Set<std::string>("version", kVersion);
    scheme_i->Save();
    ContraptionP admin = users->New();
    admin->Set<string>("name", "admin");
    admin->Set<string>("password_hash", sha256("admin"));
    admin->Set<int>("admin", true);
    admin->Save();
  }
  } catch (exception &e) {
    cerr << e.what();
  }
}

}
}
