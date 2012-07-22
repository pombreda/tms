#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testProtocol

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

// std
#include <sstream>
#include <memory>
#include <iostream>
// boost
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/test/unit_test.hpp>
// common
#include <protocol/protocol.hpp>
#include <protocol/message.hpp>
#include "dummy.pb.h"
 
using namespace std;
using namespace tms::common::protocol;
using namespace tms::test;

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

BOOST_AUTO_TEST_SUITE(testSuiteProtocol)

BOOST_FIXTURE_TEST_CASE(testStream, Fixture)
{
  Protocol protocol;
  protocol.AddMessageClass<DummyMessage>();
  protocol.AddMessageClass<DummyMessage2>();
  BOOST_CHECK(!protocol.IsInitialized());
  protocol.Initialize();
  BOOST_CHECK(protocol.IsInitialized());
  stringstream stream(ios_base::in | ios_base::out);
  DummyMessage dummy;
  dummy.set_name("hlesss");
  protocol.WriteMessage(stream, dummy);
  protocol.WriteMessage(stream, dummy);
    
  boost::shared_ptr<DummyMessage> restored 
      = boost::dynamic_pointer_cast<DummyMessage>(protocol.ReadMessage(stream));
  BOOST_CHECK(restored);
  BOOST_CHECK_EQUAL(dummy.name(), restored->name());
  boost::shared_ptr<DummyMessage2> restored2 
      = boost::dynamic_pointer_cast<DummyMessage2>(protocol.ReadMessage(stream));
  BOOST_CHECK(!restored2);
  BOOST_CHECK_THROW(protocol.WriteMessage(stream, DummyMessage3()), ProtocolException);
  protocol.WriteMessage(stream, DummyMessage2());
}

BOOST_AUTO_TEST_SUITE_END()
