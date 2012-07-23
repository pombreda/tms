#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testServerClient
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
// common
#include <protocol/protocol.hpp>
#include <protocol/simple_request_processor.hpp>
#include <protocol/tcp_server.hpp>
#include <protocol/stream_client.hpp>
#include <protocol/message.hpp>
#include "dummy.pb.h"
#include "dummy_protocol.hpp"
 
using namespace std;
using namespace tms::common::protocol;
using namespace tms::test;
using boost::asio::ip::tcp;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture() {}

  virtual ~Fixture(){}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testSuiteServerClient)

MessageP DummyHandler(const DummyMessage &message) {
  boost::shared_ptr<DummyMessage> ret(new DummyMessage(message));
  ret->set_name("Evaled: " + ret->name());
  return ret;
}

BOOST_FIXTURE_TEST_CASE(testBase, Fixture)
{
  SimpleRequestProcessorP request_processor(new SimpleRequestProcessor());
  request_processor->AddHandler(DummyHandler);
  ServerP server(
      new TCPServer(tcp::endpoint(tcp::v4(), 3030), ProtocolP(new DummyProtocol), request_processor));  
  server->Listen();
  ClientP client(
      new StreamClient(StreamP(new tcp::iostream("localhost", "3030")), 
                       ProtocolP(new DummyProtocol)));
  boost::shared_ptr<DummyMessage> message(new DummyMessage());
  message->set_name("huricane bla bla bla");
  message = boost::dynamic_pointer_cast<DummyMessage>(client->EvalRequest(*message));
  BOOST_CHECK(message);
  server->Stop();
  BOOST_CHECK_EQUAL("Evaled: huricane bla bla bla", message->name());
}

BOOST_AUTO_TEST_SUITE_END()
