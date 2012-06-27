#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testMemoryContraption

//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>//oops
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/memory_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
 
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
  boost::scoped_ptr<Contraption> test_contraprion(
      new Contraption(model()));
  BOOST_CHECK(test_contraprion);
  boost::scoped_ptr<Contraption> copy_contraprion(
      new Contraption(*test_contraprion));
  BOOST_CHECK(test_contraprion);
}

BOOST_FIXTURE_TEST_CASE(testGetFieldID, Fixture) {
  boost::shared_ptr<Model> test_model = model();
  BOOST_CHECK_EQUAL(test_model->GetFieldID("name"), 0);
  BOOST_CHECK_EQUAL(test_model->GetFieldID("age"), 1);
  BOOST_CHECK_THROW(
      test_model->GetFieldID("surname"), 
      FieldException);
  boost::scoped_ptr<Contraption> test_contraprion(
      new Contraption(model()));
  BOOST_CHECK_EQUAL(test_contraprion->GetFieldID("name"), 0);
  BOOST_CHECK_EQUAL(test_contraprion->GetFieldID("age"), 1);
  BOOST_CHECK_THROW(
      test_contraprion->GetFieldID("surname"), 
      FieldException);
  boost::scoped_ptr<Contraption> copy_contraprion(
      new Contraption(*test_contraprion));
  BOOST_CHECK_EQUAL(copy_contraprion->GetFieldID("name"), 0);
  BOOST_CHECK_EQUAL(copy_contraprion->GetFieldID("age"), 1); 
  BOOST_CHECK_THROW(
      copy_contraprion->GetFieldID("surname"), 
      FieldException);
}

BOOST_FIXTURE_TEST_CASE(testGetField, Fixture) {
  boost::shared_ptr<Model> test_model = model();
  BOOST_CHECK_EQUAL(test_model->GetField(0)->name(),
                    "name");
  BOOST_CHECK_EQUAL(test_model->GetField(1)->name(),
                    "age");
  BOOST_CHECK(dynamic_cast<const SimpleFieldT<string>*>(
      test_model->GetField(0)));
  BOOST_CHECK(dynamic_cast<const SimpleFieldT<int>*>(
      test_model->GetField(1)));
  BOOST_CHECK_THROW(
      test_model->GetField(2), 
      FieldException);
}

BOOST_FIXTURE_TEST_CASE(testSaveString, Fixture) {
  MemoryModelBackend* test_backend = backend();
  boost::shared_ptr<Model> test_model(
      new Model(fields(), test_backend));    
  boost::scoped_ptr<Contraption> test_contraption(
      new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  boost::scoped_ptr<FieldType> value(
      new FieldTypeT<string>("Dummy"));
  test_contraption->SetFieldValue("name", value.get());

  value.reset(new FieldTypeT<int>(12));
  test_contraption->SetFieldValue("age", value.get());
  
  BOOST_CHECK(accessor.id() == Contraption::kNewID);

  test_contraption->Save();

  BOOST_CHECK(accessor.id() != Contraption::kNewID);

  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"],
                    12);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));

  ContraptionID id = accessor.id();

  value.reset(new FieldTypeT<int>(14));
  test_contraption->SetFieldValue("age", value.get());
  test_contraption->Save();
  BOOST_CHECK(accessor.id() == id);
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"], 
                    14);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));

  
  boost::scoped_ptr<Contraption> test_contraption2(
      new Contraption(test_model));
  ContraptionAccessor accessor2(test_contraption2.get());
  test_contraption2->SetFieldValue("age", 
                                  FieldTypeT<int>(17));
  test_contraption2->SetFieldValue("name", 
                                  FieldTypeT<string>("Leonid"));
  test_contraption2->Save();

  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 2);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 2);

  BOOST_CHECK(accessor.id() == id);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"], 
                    14);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));

  BOOST_CHECK(accessor2.id() != id);
  BOOST_CHECK(accessor2.id() != Contraption::kNewID);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor2.id()]["age"], 
                    17);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor2.id()]["name"], 
                    string("Leonid"));

  
  value.reset(new FieldTypeT< vector<int> >());
  BOOST_CHECK_THROW(
      test_contraption->SetFieldValue("age", value.get()), 
      FieldException);
  
  BOOST_CHECK_THROW(
      test_contraption->SetFieldValue("name", 
                                      FieldTypeT<int>(17)), 
      FieldException);

  BOOST_CHECK_THROW(
      test_contraption->SetFieldValue("surname", 
                                      FieldTypeT<int>(17)), 
      FieldException);


}

BOOST_FIXTURE_TEST_CASE(testSaveInt, Fixture) {
  MemoryModelBackend* test_backend = backend();
  boost::shared_ptr<Model> test_model(
      new Model(fields(), test_backend));    
  boost::scoped_ptr<Contraption> test_contraption(
      new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  boost::scoped_ptr<FieldType> value(
      new FieldTypeT<string>("Dummy"));
  test_contraption->SetFieldValue(0, value.get());

  value.reset(new FieldTypeT<int>(12));
  test_contraption->SetFieldValue(1, value.get());
  
  BOOST_CHECK(accessor.id() == Contraption::kNewID);

  test_contraption->Save();

  BOOST_CHECK(accessor.id() != Contraption::kNewID);

  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"],
                    12);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));

  ContraptionID id = accessor.id();

  value.reset(new FieldTypeT<int>(14));
  test_contraption->SetFieldValue(1, value.get());
  test_contraption->Save();
  BOOST_CHECK(accessor.id() == id);
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"], 
                    14);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));

  
  boost::scoped_ptr<Contraption> test_contraption2(
      new Contraption(test_model));
  ContraptionAccessor accessor2(test_contraption2.get());
  test_contraption2->SetFieldValue(1, 
                                  FieldTypeT<int>(17));
  test_contraption2->SetFieldValue(0, 
                                  FieldTypeT<string>("Leonid"));
  test_contraption2->Save();

  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 2);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 2);

  BOOST_CHECK(accessor.id() == id);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"], 
                    14);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));

  BOOST_CHECK(accessor2.id() != id);
  BOOST_CHECK(accessor2.id() != Contraption::kNewID);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor2.id()]["age"], 
                    17);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor2.id()]["name"], 
                    string("Leonid"));

  
  value.reset(new FieldTypeT< vector<int> >());
  BOOST_CHECK_THROW(
      test_contraption->SetFieldValue(1, value.get()), 
      FieldException);
  BOOST_CHECK_THROW(
      test_contraption->SetFieldValue(2, 
                                      FieldTypeT<int>(17)), 
      FieldException);
  BOOST_CHECK_THROW(
      test_contraption->SetFieldValue(0, 
                                      FieldTypeT<int>(17)), 
      FieldException);
}


BOOST_FIXTURE_TEST_CASE(testRefreshGetString, Fixture) {
  MemoryModelBackend* test_backend = backend();
  boost::shared_ptr<Model> test_model(
      new Model(fields(), test_backend));    
  boost::scoped_ptr<Contraption> test_contraption(
      new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  boost::scoped_ptr<FieldType> value(
      new FieldTypeT<string>("Dummy"));
  test_contraption->SetFieldValue("name", value.get());

  value.reset(new FieldTypeT<int>(12));
  test_contraption->SetFieldValue("age", value.get());
  
  test_contraption->Save();

  test_backend->int_fields()[accessor.id()]["age"] = 18;
  int age = dynamic_cast<FieldTypeT<int>*>(
      test_contraption->GetFieldValue("age"))->data();
  string name = dynamic_cast<FieldTypeT<string>*>(
      test_contraption->GetFieldValue("name"))->data();
  
  BOOST_CHECK_EQUAL(age, 12);
  BOOST_CHECK_EQUAL(name, string("Dummy"));
  
  test_contraption->Refresh();
  age = dynamic_cast<FieldTypeT<int>*>(
      test_contraption->GetFieldValue("age"))->data();
  name = dynamic_cast<FieldTypeT<string>*>(
      test_contraption->GetFieldValue("name"))->data();
  BOOST_CHECK_EQUAL(age, 18);
  BOOST_CHECK_EQUAL(name, string("Dummy"));
  BOOST_CHECK(accessor.id() != Contraption::kNewID);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"], 
                    18);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_THROW(test_contraption->GetFieldValue("surname"),
                    FieldException);
}

BOOST_FIXTURE_TEST_CASE(testRefreshGetInt, Fixture) {
  MemoryModelBackend* test_backend = backend();
  boost::shared_ptr<Model> test_model(
      new Model(fields(), test_backend));    
  boost::scoped_ptr<Contraption> test_contraption(
      new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  boost::scoped_ptr<FieldType> value(
      new FieldTypeT<string>("Dummy"));
  test_contraption->SetFieldValue("name", value.get());

  value.reset(new FieldTypeT<int>(12));
  test_contraption->SetFieldValue("age", value.get());
  
  test_contraption->Save();

  test_backend->int_fields()[accessor.id()]["age"] = 18;
  int age = dynamic_cast<FieldTypeT<int>*>(
      test_contraption->GetFieldValue(1))->data();
  string name = dynamic_cast<FieldTypeT<string>*>(
      test_contraption->GetFieldValue(0))->data();
  
  BOOST_CHECK_EQUAL(age, 12);
  BOOST_CHECK_EQUAL(name, string("Dummy"));
  
  test_contraption->Refresh();
  age = dynamic_cast<FieldTypeT<int>*>(
      test_contraption->GetFieldValue(1))->data();
  name = dynamic_cast<FieldTypeT<string>*>(
      test_contraption->GetFieldValue(0))->data();
  BOOST_CHECK_EQUAL(age, 18);
  BOOST_CHECK_EQUAL(name, string("Dummy"));
  BOOST_CHECK(accessor.id() != Contraption::kNewID);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"], 
                    18);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"], 
                    string("Dummy"));
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);
  BOOST_CHECK_THROW(test_contraption->GetFieldValue(2),
                    FieldException);
}

BOOST_FIXTURE_TEST_CASE(testGetFieldIDName, Fixture) {
  boost::scoped_ptr<Contraption> test_contraption(
      new Contraption(model()));
  BOOST_CHECK_EQUAL(test_contraption->GetFieldName(
      test_contraption->GetFieldID("name")), "name");
  BOOST_CHECK_EQUAL(test_contraption->GetFieldName(
      test_contraption->GetFieldID("age")), "age");
  BOOST_CHECK_THROW(test_contraption->GetFieldName(2),
                    FieldException);
  BOOST_CHECK_THROW(test_contraption->GetFieldID("surname"),
                    FieldException);
}
BOOST_AUTO_TEST_SUITE_END()
