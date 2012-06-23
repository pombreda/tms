#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testRequest

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <protocol/request.hpp>
#include <protocol/crypto.hpp>
#include <setup.hpp>
#include <sstream>
#include <boost/test/unit_test.hpp>
 
using namespace std;
using namespace tms::common;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture()
      : user_simple("adavydow"),
        password_simple("qwerty"),
        request_simple(user_simple, password_simple),
        user_hard(hard_name()),
        password_hard("qwerty"),
        request_hard(user_hard, password_hard) {
  }
  static string hard_name() {
    ostringstream ostr(ostringstream::binary);
    ostr << '\0' << '\\' << '\0' << '\1' << '\\' << '\\'
         << "sdfaga" << '\0' << '\0' << '\\' << '\0' << '\\'
         << endl << '\0' << "eee";
    return ostr.str();
  }

  string user_simple;
  string password_simple;
  Request request_simple;
  string user_hard;
  string password_hard;
  Request request_hard;
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testSuiteRequest)

BOOST_FIXTURE_TEST_CASE(testUser, Fixture)
{
  BOOST_CHECK_EQUAL(
      request_simple.user(), 
      string(user_simple)
                    );
}

BOOST_FIXTURE_TEST_CASE(testPassword, Fixture)
{  
  BOOST_CHECK_EQUAL(
      request_simple.password_hash(), 
      sha256(password_simple)
                    );  
}

BOOST_FIXTURE_TEST_CASE(testVersion, Fixture)
{
  BOOST_CHECK_EQUAL(
      request_simple.version(), 
      kVersion
                    );    
}

BOOST_FIXTURE_TEST_CASE(testSerializeDeserializeSimple, Fixture)
{
  ostringstream sout(ostringstream::binary);
  request_simple.Serialize(sout);
  istringstream sin(sout.str(), istringstream::binary);
  Request *req = Request::Deserialize(sin);
  BOOST_CHECK_EQUAL(
      req->user(), 
      string(user_simple)
                    );
  BOOST_CHECK_EQUAL(
      req->password_hash(), 
      sha256(password_simple)
                    );  
  BOOST_CHECK_EQUAL(
      req->version(), 
      kVersion
                    );    
}

BOOST_FIXTURE_TEST_CASE(testSerializeDeserializeHard, Fixture)
{
  ostringstream sout(ostringstream::binary);
  request_hard.Serialize(sout);
  istringstream sin(sout.str(), istringstream::binary);
  Request *req = Request::Deserialize(sin);
  BOOST_CHECK_EQUAL(
      req->user(), 
      string(user_hard)
                    );
  BOOST_CHECK_EQUAL(
      req->password_hash(), 
      sha256(password_hard)
                    );  
  BOOST_CHECK_EQUAL(
      req->version(), 
      kVersion
                    );    
}


BOOST_AUTO_TEST_SUITE_END()
