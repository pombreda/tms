//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testGUIException

#include <gui_exception/gui_exception.hpp>
#include <boost/test/unit_test.hpp>
 
using namespace std;
using namespace tms::common;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  class DummyException : public exception {
    virtual const char *what() const throw() {
      return "DummyException.";
    }
  };
  Fixture() :
      dummy(),
      from_message("Exception Occured."),
      from_exception(&dummy),
      from_exception_message(&dummy, "Exception was thrown."),
      from_gui_exception(&from_message, "GUIException was thrown.")
  {
  }
  DummyException dummy;
  GUIException from_message;
  GUIException from_exception;
  GUIException from_exception_message;
  GUIException from_gui_exception;
  virtual ~Fixture(){}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testSuiteGUIException)

BOOST_FIXTURE_TEST_CASE(testWhat, Fixture)
{
  BOOST_CHECK_EQUAL(
      string(from_message.what()), 
      string("GUIException. To obtain full exception message use ::Message()")
                    );
  BOOST_CHECK_EQUAL(
      string(from_exception.what()), 
      string("GUIException. To obtain full exception message use ::Message()")
                    );
  BOOST_CHECK_EQUAL(
      string(from_exception_message.what()), 
      string("GUIException. To obtain full exception message use ::Message()")
                    );
  BOOST_CHECK_EQUAL(
      string(from_gui_exception.what()), 
      string("GUIException. To obtain full exception message use ::Message()")
                    );
}

BOOST_FIXTURE_TEST_CASE(testMessage, Fixture)
{  
  BOOST_CHECK_EQUAL(
      GUIException(from_message).message(), 
      string("Exception Occured.")
                    );
  BOOST_CHECK_EQUAL(
      from_exception.message(), 
      string("DummyException.")
                    );
  BOOST_CHECK_EQUAL(
      from_exception_message.message(), 
      string("Exception was thrown.\n==\nDummyException.")
                    );
  BOOST_CHECK_EQUAL(
      string(from_gui_exception.message()), 
      string("GUIException was thrown.\n==\nException Occured.")
                    );
}

BOOST_FIXTURE_TEST_CASE(testCopy, Fixture)
{
  BOOST_CHECK_EQUAL(
      GUIException(from_message).message(), 
      string("Exception Occured.")
                    );
  BOOST_CHECK_EQUAL(
      GUIException(from_exception).message(), 
      string("DummyException.")
                    );
  BOOST_CHECK_EQUAL(
      GUIException(from_exception_message).message(), 
      string("Exception was thrown.\n==\nDummyException.")
                    );
  BOOST_CHECK_EQUAL(
      string(GUIException(from_gui_exception).message()), 
      string("GUIException was thrown.\n==\nException Occured.")
                    );
}


BOOST_AUTO_TEST_SUITE_END()
