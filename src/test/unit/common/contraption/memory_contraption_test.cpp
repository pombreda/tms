#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testMemoryContraption

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/memory_model_backend.hpp>
 
using namespace std;
using namespace tms::common::contraption;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture() {
  }
  MemoryModelBackend *model_backend() {
    return new MemoryModelBackend();
  }
  virtual ~Fixture(){}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testMemoryContraption)

BOOST_FIXTURE_TEST_CASE(testMemoryModelBackendConstructor, Fixture) {
  BOOST_CHECK(model_backend() != 0);

}

BOOST_AUTO_TEST_SUITE_END()
