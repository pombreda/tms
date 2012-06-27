#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testMemoryContraption

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/memory_model_backend.hpp>
#include <contraption/field.hpp>
 
using namespace std;
using namespace tms::common::contraption;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture() {
  }

  MemoryModelBackend* backend() {
    return new MemoryModelBackend();
  }

  vector<Field*> fields() {
    vector<Field*> ret;
    ret.push_back(new SimpleFieldT<string>("name"));
    ret.push_back(new SimpleFieldT<int>("age"));
    return ret;
  }

  boost::shared_ptr<Model> model() {
    return boost::shared_ptr<Model>(new Model(fields(), 
                                              backend()));    
  }

  virtual ~Fixture() {}
};

//------------------------------------------------------------
// Tests
//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(testMemoryContraption)

BOOST_FIXTURE_TEST_CASE(testMemoryModelBackendConstructor, Fixture) {
  boost::shared_ptr<ModelBackend> test_backend(backend());
  BOOST_CHECK(test_backend);
}

BOOST_FIXTURE_TEST_CASE(testSimpleFieldTConstructor, Fixture) {
  {
    boost::scoped_ptr<Field> test_field(new SimpleFieldT<int>("name", 
                                                              true));
    BOOST_CHECK(test_field);
  }
  {
    boost::scoped_ptr<Field> test_field(new SimpleFieldT<int>("name", 
                                                              false));
    BOOST_CHECK(test_field);
  }
  {
    boost::scoped_ptr<Field> test_field(new SimpleFieldT<int>("name"));
    BOOST_CHECK(test_field);
  }
  {
    boost::scoped_ptr<Field> test_field(new SimpleFieldT<string>("name"));
    BOOST_CHECK(test_field);
  }

  {
    boost::scoped_ptr<Field> test_field(new SimpleFieldT<string>("name", 
                                                                 true));
    BOOST_CHECK(test_field);
  }
  {
    boost::scoped_ptr<Field> test_field(new SimpleFieldT<string>("name", 
                                                                 false));
    BOOST_CHECK(test_field);
  }
  BOOST_CHECK_THROW(SimpleFieldT<int>(""), FieldException);
  BOOST_CHECK_THROW(SimpleFieldT<string>(""), FieldException);
}

BOOST_FIXTURE_TEST_CASE(testModelConstructor, Fixture) {
  {
    boost::scoped_ptr<Model> test_model(new Model(vector<Field*>(), 
                                                  backend()));
    BOOST_CHECK(test_model);
  }
  {
    boost::scoped_ptr<Model> test_model(new Model(vector<Field*>(), 
                                                  new MemoryModelBackend()));
    BOOST_CHECK(test_model);
  }
  {
    boost::scoped_ptr<Model> test_model(new Model(fields(), 
                                                  new MemoryModelBackend()));
    BOOST_CHECK(test_model);
  }
  BOOST_CHECK(model());
}

BOOST_FIXTURE_TEST_CASE(testModelGetFieldNumber, Fixture) {
  {
    boost::scoped_ptr<Model> test_model(new Model(vector<Field*>(), 
                                                  new MemoryModelBackend()));
    BOOST_CHECK_EQUAL(test_model->GetFieldNumber(), 0);
  }
  {
    boost::scoped_ptr<Model> test_model(new Model(fields(), 
                                                  new MemoryModelBackend()));
    BOOST_CHECK_EQUAL(test_model->GetFieldNumber(), fields().size());
  }
  {
    vector<Field*> test_fields;
    test_fields.push_back(new SimpleFieldT<string>("name"));
    test_fields.push_back(new SimpleFieldT<string>("surname"));
    test_fields.push_back(new SimpleFieldT<int>("age"));
    boost::scoped_ptr<Model> test_model(new Model(test_fields, 
                                                  new MemoryModelBackend()));
    BOOST_CHECK_EQUAL(test_model->GetFieldNumber(), test_fields.size());
  }
  {
    vector<Field*> test_fields;
    test_fields.push_back(new SimpleFieldT<string>("name"));
    test_fields.push_back(new SimpleFieldT<string>("name"));
    test_fields.push_back(new SimpleFieldT<int>("age"));
    BOOST_CHECK_THROW(
        Model(test_fields, new MemoryModelBackend()), 
        FieldException);
  }
}

BOOST_FIXTURE_TEST_CASE(testContraptionConstructor, Fixture) {
  {
    boost::scoped_ptr<Contraption> test_contraprion(
        new Contraption(model()));
  }
}

BOOST_AUTO_TEST_SUITE_END()
