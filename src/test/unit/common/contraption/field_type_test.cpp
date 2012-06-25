//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testFieldType

#include <contraption/field_type.hpp>

#include <boost/test/unit_test.hpp>


using namespace std;
using namespace tms::common::contraption;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture() {
  }
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

TMS_FIELD_TYPE_INIT_LIB(0)
INIT_FIELD_TYPE(int, 0, 0)
INIT_FIELD_TYPE(string, 0, 1)

BOOST_AUTO_TEST_SUITE(testFieldType)

BOOST_FIXTURE_TEST_CASE(testNewDelete, Fixture)
{
  void *string_ptr = FieldTypeT<string>().New();
  BOOST_CHECK_EQUAL(
      *static_cast<string *>(string_ptr),
      string()
                    );
  FieldTypeT<string>().Free(string_ptr);
  void *int_ptr = FieldTypeT<int>().New();
  BOOST_CHECK_EQUAL(
      *static_cast<int *>(int_ptr),
      int()
                    );
  FieldTypeT<int>().Free(int_ptr);
}

BOOST_FIXTURE_TEST_CASE(testDuplicate, Fixture)
{
  string s1 = "hello";
  void *string_ptr = FieldTypeT<string>().Duplicate(static_cast<void *>(&s1));
  BOOST_CHECK_EQUAL(
      *static_cast<string *>(string_ptr),
      string("hello")
                    );
  FieldTypeT<string>().Free(string_ptr);
  int i1 = 12413;
  void *int_ptr = FieldTypeT<int>().Duplicate(static_cast<void *>(&i1));
  BOOST_CHECK_EQUAL(
      *static_cast<int *>(int_ptr),
      12413
                    );
  FieldTypeT<int>().Free(int_ptr);
}

BOOST_FIXTURE_TEST_CASE(testID, Fixture)
{
  BOOST_CHECK_EQUAL(
      FieldTypeT<string>().id(),
      FieldTypeT<string>().id()
                    );
  BOOST_CHECK_EQUAL(
      FieldTypeT<int>().id(),
      FieldTypeT<int>().id()
                    );
  BOOST_CHECK(
      FieldTypeT<int>().id() != FieldTypeT<string>().id()
              );
} 


BOOST_AUTO_TEST_SUITE_END()
