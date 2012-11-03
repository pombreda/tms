#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testServerContraption

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
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
#include <contraption/field/has_many_field.hpp>
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
#include <string/string.hpp>
#include <model/user.hpp>

using namespace std;
using namespace tms::common::contraption;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;
using namespace tms::common::model;
using namespace tms::common::string;
using boost::asio::ip::tcp;
using namespace log4cplus;

struct InitLog {
  InitLog() {
    PropertyConfigurator config(WStringFromUTF8String("log.cfg"));
    config.configure();
  }
} init_log;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture()
      : backend(),
        model(),
        users(),
        server(),
        client() {
          cerr << "Fixture init" << endl;
    // DB data
    string test_db("test.sqlite3");
    remove(test_db.c_str());
    SOCIDBScheme scheme(soci::sqlite3, test_db);
    // InitSchema
    users = User::GetModel(ModelBackendP(new SOCIModelBackend(scheme, "users")));
    users->InitSchema();
    users.reset();
    vector<Field*> fields;
    fields.push_back(new SimpleFieldT<string>("name"));
    fields.push_back(new SimpleFieldT<int>("age"));
    fields.push_back(new SimpleFieldT<int>("password",
                                           _is_readable = false));
    fields.push_back(new SimpleFieldT<string>("Surname",
                                              _backend_name = "surname"));
    soci_model.reset(new Model(fields, new SOCIModelBackend(scheme, "test")));
    soci_model->InitSchema();
    soci_model.reset();

    fields.clear();
    fields.push_back(new IntField("id"));
    fields.push_back(new IntField("other_id"));
    through_model.reset(new Model(fields,
                                  new SOCIModelBackend(scheme, "test_con")));
    through_model->InitSchema();
    // Init Models
    users = User::GetModel(ModelBackendP(new SOCIModelBackend(scheme, "users")));
    fields.clear();
    fields.push_back(new StringField("name"));
    fields.push_back(new IntField("age"));
    fields.push_back(new IntField("password",
                                  _is_readable = false));
    fields.push_back(new StringField("Surname",
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
    ModelBackendRequestProcessor::Register(*request_processor, scheme);
    RequestProcessorP processor(
        new LoginRequestProcessor(request_processor, users));
    // Create Server
    server.reset(
        new TCPServer("3030",
                      protocol,
                      processor));
    server->Listen();
    // Create Client
    client.reset(
        new SocketClient("localhost", "3030",
                         protocol));
    LoginRequestP login(new LoginRequest);
    login->set_name("adavydow");
    login->set_password_hash("Dummy");
    MessageP ret = client->EvalRequest(*login);
    BOOST_CHECK(boost::dynamic_pointer_cast<LoginResponse>(ret));
    BOOST_CHECK(boost::dynamic_pointer_cast<LoginResponse>(ret)->status()
                == LoginResponse::kOk);
    // init through model
    fields.clear();
    fields.push_back(new IntField("id"));
    fields.push_back(new IntField("other_id"));
    through_model.reset(new Model(fields,
                                  new ServerModelBackend(client, "test_con")));

    // Create Backend
    backend.reset(new ServerModelBackend(client, "test"));
    // Init model
    fields.clear();
    model.reset(new Model(backend));
    fields.push_back(new StringField("name"));
    fields.push_back(new IntField("age"));
    fields.push_back(new IntField("password",
                                  _is_readable = false));
    fields.push_back(new StringField("Surname",
                                     _backend_name = "surname"));
    fields.push_back(new HasManyField("friends",
                                      boost::ref(*model),
                                      boost::ref(*through_model)));
    model->InitFields(fields);

  }

  ModelBackendP backend;
  ModelP model;
  ModelP through_model;
  ModelP soci_model;
  ModelP users;
  ServerP server;
  ClientP client;

  virtual ~Fixture() {
    server->Stop();
  }
};


//------------------------------------------------------------
// Tests
//------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(testServerContraption)

BOOST_FIXTURE_TEST_CASE(testUseCase, Fixture) {
  ContraptionP test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Du'\"\\mmy");
  test_contraption->Save();
  ContraptionArrayP contraptions = model->All();
  contraptions->at(0)->Delete();
  test_contraption->Delete();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Dustrmmy");
  test_contraption->Save();
  contraptions = model->All();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    1);
  contraptions->Save();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    1);
  contraptions->Refresh();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    1);
  contraptions->Save();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    1);
  contraptions->at(0)->Delete();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Du'\"\\mmy");
  test_contraption->Save();
  ContraptionP test_contraption2 = model->New();
  test_contraption2->Set<int>("age", 12);
  test_contraption2->Set<string>("Surname", "Ymmud");
  test_contraption2->Save();
  test_contraption->Save();
  cerr << 1 << endl;
  ContraptionArrayP friends
      = test_contraption2->Get<ContraptionArrayP>("friends");
  cerr << 2 << endl;
  friends->push_back(test_contraption2);
  friends->push_back(test_contraption);
  cerr << 3 << endl;
  test_contraption2->Save();
  contraptions->Refresh();


  BOOST_CHECK_EQUAL(contraptions->size(),
                    2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Du'\"\\mmy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<ContraptionArrayP>("friends")->size(),
                    0);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<ContraptionArrayP>("friends")->size(),
                    2);

  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");

  contraptions->erase(0);
  contraptions->Save();
  contraptions = model->All();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    1);
  test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Dummy");
  contraptions->push_back(test_contraption);
  contraptions->Save();
  contraptions = model->All();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    2);
}

BOOST_FIXTURE_TEST_CASE(testFilter, Fixture) {
  const SimpleFieldT<int> *age
      = dynamic_cast<const SimpleFieldT<int>*>(
          model->GetField("age"));
  const SimpleFieldT<string> *surname
      = dynamic_cast<const SimpleFieldT<string>*>(
          model->GetField("Surname"));
  BOOST_CHECK(age);
  ContraptionP test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Dummy");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 12);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 12);
  test_contraption->Set<string>("Surname", "Dummy");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 12);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 14);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
  // Simple string
  FilterP filter = Compare(surname, kEqual, string("Dummy"));
  ContraptionArrayP contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);

  // Simple int
  filter = Compare(age, kEqual, 12);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 3);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  filter = Compare(age, kNotLesser, 12);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 4);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<int>("age"),
                    14);
  filter = Compare(age, kGreater, 10);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 4);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<int>("age"),
                    14);
  filter = Compare(age, kGreater, 12);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 1);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    14);
  filter = Compare(age, kNotEqual, 12);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 3);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    14);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    10);
  filter = Compare(age, kLesser, 12);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    10);
  filter = Compare(age, kNotGreater, 12);
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 5);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(4)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(4)->Get<int>("age"),
                    10);
  // Not int
  filter = Not(Compare(age, kNotEqual, 12));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 3);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  filter = Not(Compare(age, kLesser, 12));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 4);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<int>("age"),
                    14);
  filter = Not(Compare(age, kNotGreater, 10));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 4);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<int>("age"),
                    14);
  filter = Not(Compare(age, kNotGreater, 12));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 1);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    14);
  filter = Not(Compare(age, kEqual, 12));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 3);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    14);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    10);
  filter = Not(Compare(age, kNotLesser, 12));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    10);
  filter = Not(Compare(age, kGreater, 12));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 5);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(2)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(3)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(4)->Get<string>("Surname"),
                    "Ymmud");
  BOOST_CHECK_EQUAL(contraptions->at(4)->Get<int>("age"),
                    10);
  // AND
  filter = And(Compare(age, kNotGreater, 12),
               Compare(surname, kEqual, string("Dummy")));
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  // OR
  filter = Not(
      Or(
          Not(Compare(age, kNotGreater, 12)),
          Not(Compare(surname, kEqual, string("Dummy")))
         )
               );
  contraptions = model->Filter(filter);
  BOOST_CHECK_EQUAL(contraptions->size(), 2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
}

BOOST_AUTO_TEST_SUITE_END()
