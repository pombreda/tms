#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testSociContraption

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <cstdio>
// boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>//oops
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>

using namespace std;
using namespace tms::common::contraption;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture()
    : backend(),
      model() {
    string test_db("test.sqlite3");
    remove(test_db.c_str());
    SOCIDBScheme scheme(soci::sqlite3, test_db);    
    backend.reset(new SOCIModelBackend(scheme, "test"));    
    vector<Field*> fields;
    fields.push_back(new SimpleFieldT<string>("name"));
    fields.push_back(new SimpleFieldT<int>("age"));
    fields.push_back(new SimpleFieldT<int>("password", 
                                           _is_readable = false));
    fields.push_back(new SimpleFieldT<string>("Surname", 
                                              _backend_name = "surname"));
    model.reset(new Model(fields, backend));    
    model->InitSchema();
  }

  ModelBackendP backend;
  ModelP model;


  virtual ~Fixture() {}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(testSociContraption)

BOOST_FIXTURE_TEST_CASE(testUseCase, Fixture) {
  ContraptionP test_contraption = model->New();
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Du'\"\\mmy");
  test_contraption->Save();
  test_contraption = model->New();
  test_contraption->Set<int>("age", 12);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
  ContraptionArrayP contraptions = model->All();
  BOOST_CHECK_EQUAL(contraptions->size(), 
                    2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"), 
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"), 
                    "Du'\"\\mmy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"), 
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"), 
                    "Ymmud");
  contraptions->erase(contraptions->begin());
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
