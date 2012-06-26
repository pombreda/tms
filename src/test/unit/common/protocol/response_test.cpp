#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testRequest

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <protocol/response.hpp>
#include <setup.hpp>
#include <sstream>
#include <memory>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/test/unit_test.hpp>
 
using namespace std;
using namespace tms::common;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture() {
  }
  virtual ~Fixture(){}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testSuiteResponse)

BOOST_FIXTURE_TEST_CASE(testStatus, Fixture)
{
  for (size_t status = 0; status < 300; ++status) {
    BOOST_CHECK_EQUAL(
        Response(status).status(), 
        status
                      );
    
  }
}

BOOST_FIXTURE_TEST_CASE(testSerializeDeserializeSimple, Fixture)
{
  for (size_t status = 0; status < 300; ++status) {
    ostringstream sout(ostringstream::binary);
    boost::archive::binary_oarchive oa(sout);
    Response resp(status);
    oa << resp;
    istringstream sin(sout.str(), istringstream::binary);
    boost::archive::binary_iarchive ia(sin);
    Response req;
    ia >> req;
    
    BOOST_CHECK_EQUAL(
        req.status(), 
        status
                      );
  }
}


BOOST_AUTO_TEST_SUITE_END()
