#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testLoginServer
//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <sstream>
#include <memory>
#include <iostream>
// boost
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// log4cplus
#include <log4cplus/configurator.h>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <protocol/login_protocol.hpp>
#include <protocol/simple_request_processor.hpp>
#include <protocol/login_request_processor.hpp>
#include <protocol/tcp_server.hpp>
#include <protocol/stream_client.hpp>
#include <protocol/message.hpp>
#include <protocol/message/login_request.hpp>
#include <protocol/message/login_response.hpp>
#include <protocol/message/error_response.hpp>
#include <model/user.hpp>
#include "dummy.pb.h"
#include "dummy_protocol.hpp"
 
using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;
using namespace tms::common::contraption;
using namespace tms::test;
using namespace tms::common::model;
using boost::asio::ip::tcp;
using namespace log4cplus;

struct InitLog {
  InitLog() {
    PropertyConfigurator config("log.cfg");
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
      users() {
    string test_db("test.sqlite3");
    remove(test_db.c_str());
    SOCIDBScheme scheme(soci::sqlite3, test_db);    
    backend.reset(new SOCIModelBackend(scheme, "test"));    
    users.reset(new User(backend));    
    users->InitSchema();
    ContraptionP test_contraption = users->New();
    test_contraption->Set<string>("name", "adavydow");
    test_contraption->Set<string>("password_hash", "Dummy");
    test_contraption->Save();
  }

  ModelBackendP backend;
  ModelP users;
  virtual ~Fixture() {}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testSuiteLoginServer)

MessageP DummyHandler(const DummyMessage &message) {
  boost::shared_ptr<DummyMessage> ret(new DummyMessage(message));
  ret->set_name("Evaled: " + ret->name());
  return ret;
}

BOOST_FIXTURE_TEST_CASE(testBase, Fixture)
{
  SimpleRequestProcessorP request_processor(new SimpleRequestProcessor());
  request_processor->AddHandler(DummyHandler);
  RequestProcessorP processor(new LoginRequestProcessor(request_processor, users));
  ProtocolP protocol(new LoginProtocol(DummyProtocol()));
  protocol->Initialize();
  ServerP server(
      new TCPServer(tcp::endpoint(tcp::v4(), 3030), 
                    protocol, 
                    processor));  
  server->Listen();
  ClientP client(
      new StreamClient(StreamP(new tcp::iostream("localhost", "3030")), 
                       protocol));
  boost::shared_ptr<DummyMessage> message(new DummyMessage());
  message->set_name("huricane bla bla bla");
  MessageP ret = client->EvalRequest(*message);
  BOOST_CHECK(boost::dynamic_pointer_cast<ErrorResponse>(ret));
  BOOST_CHECK(boost::dynamic_pointer_cast<ErrorResponse>(ret)->status()
              == ErrorResponse::kAccessViolation);
  LoginRequestP login(new LoginRequest);
  login->set_name("adavydow");
  login->set_password_hash("tsr");
  ret = client->EvalRequest(*login);
  BOOST_CHECK(boost::dynamic_pointer_cast<ErrorResponse>(ret));
  BOOST_CHECK(boost::dynamic_pointer_cast<ErrorResponse>(ret)->status()
              == ErrorResponse::kAccessViolation);
  login->set_name("alex");
  login->set_password_hash("Dummy");
  ret = client->EvalRequest(*login);
  BOOST_CHECK(boost::dynamic_pointer_cast<ErrorResponse>(ret));
  BOOST_CHECK(boost::dynamic_pointer_cast<ErrorResponse>(ret)->status()
              == ErrorResponse::kAccessViolation);
  login->set_name("adavydow");
  login->set_password_hash("Dummy");
  ret = client->EvalRequest(*login);
  BOOST_CHECK(boost::dynamic_pointer_cast<LoginResponse>(ret));
  BOOST_CHECK(boost::dynamic_pointer_cast<LoginResponse>(ret)->status()
              == LoginResponse::kOk);
  message = boost::dynamic_pointer_cast<DummyMessage>(client->EvalRequest(*message));
  BOOST_CHECK(message);
  BOOST_CHECK_EQUAL("Evaled: huricane bla bla bla", message->name());
  server->Stop();
}

BOOST_AUTO_TEST_SUITE_END()
