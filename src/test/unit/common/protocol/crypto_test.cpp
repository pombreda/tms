#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testCrypto

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

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
      : simple_string("qwerty"),
        simple_string_sha256("65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5") {
  }
  string simple_string;
  string simple_string_sha256;
  virtual ~Fixture(){}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testSuiteCrypto)

BOOST_FIXTURE_TEST_CASE(testSHA256, Fixture)
{
  BOOST_CHECK_EQUAL(
      sha256(simple_string),
      simple_string_sha256
                    );
}

BOOST_AUTO_TEST_SUITE_END()
