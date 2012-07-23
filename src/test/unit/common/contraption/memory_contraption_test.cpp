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
#include <contraption/field/simple_field.hpp>
#include <contraption/field_type.hpp>
#include <contraption/contraption_accessor.hpp>
#include <contraption/contraption_array.hpp>

using namespace std;
using namespace tms::common::contraption;

//------------------------------------------------------------
// Fixture
//------------------------------------------------------------

class Fixture {
 public:
  Fixture() {
  }

  boost::shared_ptr<MemoryModelBackend> backend() {
    return boost::shared_ptr<MemoryModelBackend>(
        new MemoryModelBackend());
  }

  vector<Field*> fields() {
    vector<Field*> ret;
    ret.push_back(new SimpleFieldT<string>("name"));
    ret.push_back(new SimpleFieldT<int>("age"));
    ret.push_back(new SimpleFieldT<int>("password",
                                        _is_readable = false,
                                        _is_writable = false));
    ret.push_back(new SimpleFieldT<string>("Surname",
                                           _backend_name = "surname"));
    return ret;
  }

  size_t fields_size() {
    return 4;
  }

  ModelP model() {
    return ModelP(new Model(fields(), backend()));
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
  Model(fields(), backend());
  BOOST_CHECK(model());
  {
    ModelP test_model(new Model(vector<Field*>(),
                                backend()));
    BOOST_CHECK(test_model);
  }
  {
    ModelP test_model(new Model(vector<Field*>(),
                                new MemoryModelBackend()));
    BOOST_CHECK(test_model);
  }
  {
    ModelP test_model(new Model(fields(),
                                new MemoryModelBackend()));
    BOOST_CHECK(test_model);
  }
}

BOOST_FIXTURE_TEST_CASE(testModelGetFieldNumber, Fixture) {
  {
    ModelP test_model(new Model(vector<Field*>(),
                                new MemoryModelBackend()));
    BOOST_CHECK_EQUAL(test_model->GetFieldNumber(), 0);
  }
  {
    ModelP test_model(new Model(fields(),
                                new MemoryModelBackend()));
    BOOST_CHECK_EQUAL(test_model->GetFieldNumber(), fields_size());
  }
  {
    vector<Field*> test_fields;
    test_fields.push_back(new SimpleFieldT<string>("name"));
    test_fields.push_back(new SimpleFieldT<string>("surname"));
    test_fields.push_back(new SimpleFieldT<int>("age"));
    ModelP test_model(new Model(test_fields,
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
  ContraptionP test_contraprion(new Contraption(model()));
  BOOST_CHECK(test_contraprion);
  ContraptionP copy_contraprion(new Contraption(*test_contraprion));
  BOOST_CHECK(test_contraprion);
}

BOOST_FIXTURE_TEST_CASE(testGetFieldID, Fixture) {
  ModelP test_model = model();
  BOOST_CHECK_EQUAL(test_model->GetFieldID("name"), 0);
  BOOST_CHECK_EQUAL(test_model->GetFieldID("age"), 1);
  BOOST_CHECK_THROW(
      test_model->GetFieldID("surname"),
      FieldException);
  ContraptionP test_contraprion(new Contraption(model()));
  BOOST_CHECK_EQUAL(test_contraprion->GetID("name"), 0);
  BOOST_CHECK_EQUAL(test_contraprion->GetID("age"), 1);
  BOOST_CHECK_THROW(
      test_contraprion->GetID("surname"),
      FieldException);
  ContraptionP copy_contraprion(new Contraption(*test_contraprion));
  BOOST_CHECK_EQUAL(copy_contraprion->GetID("name"), 0);
  BOOST_CHECK_EQUAL(copy_contraprion->GetID("age"), 1);
  BOOST_CHECK_THROW(
      copy_contraprion->GetID("surname"),
      FieldException);
}

BOOST_FIXTURE_TEST_CASE(testGetField, Fixture) {
  ModelP test_model = model();
  BOOST_CHECK_EQUAL(test_model->GetField(0)->name(),
                    "name");
  BOOST_CHECK_EQUAL(test_model->GetField(1)->name(),
                    "age");
  BOOST_CHECK(dynamic_cast<const SimpleFieldT<string>*>(
      test_model->GetField(0)));
  BOOST_CHECK(dynamic_cast<const SimpleFieldT<int>*>(
      test_model->GetField(1)));
  BOOST_CHECK_THROW(
      test_model->GetField(4),
      FieldException);
}

BOOST_FIXTURE_TEST_CASE(testSaveString, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend = backend();
  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  test_contraption->Set<string>("name", "Dummy");
  test_contraption->Set("age", 12);

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

  test_contraption->Set("age", 14);
  test_contraption->Save();
  BOOST_CHECK(accessor.id() == id);
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"],
                    14);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"],
                    string("Dummy"));


  ContraptionP test_contraption2(
      new Contraption(test_model));
  ContraptionAccessor accessor2(test_contraption2.get());
  test_contraption2->Set("age", 17);
  test_contraption2->Set<string>("name", "Leonid");
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


  BOOST_CHECK_THROW(
      test_contraption->Set("age", vector<int>()),
      FieldException);


  BOOST_CHECK_THROW(
      test_contraption->Set("name", 17),
      FieldException);

  BOOST_CHECK_THROW(
      test_contraption->Set("surname", 17),
      FieldException);
}

BOOST_FIXTURE_TEST_CASE(testSaveInt, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend
      = backend();

  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  test_contraption->Set<string>(0, "Dummy");
  test_contraption->Set(1, 12);

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

  test_contraption->Set(1, 14);
  test_contraption->Save();
  BOOST_CHECK(accessor.id() == id);
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"],
                    14);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"],
                    string("Dummy"));


  ContraptionP test_contraption2(new Contraption(test_model));
  ContraptionAccessor accessor2(test_contraption2.get());
  test_contraption2->Set(1, 17);
  test_contraption2->Set<string>(0, "Leonid");
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


  BOOST_CHECK_THROW(
      test_contraption->Set(1, vector<int>()),
      FieldException);
  BOOST_CHECK_THROW(
      test_contraption->Set(4, 17),
      FieldException);
  BOOST_CHECK_THROW(
      test_contraption->Set(0, 17),
      FieldException);
}


BOOST_FIXTURE_TEST_CASE(testRefreshGetString, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend
      = backend();
  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());
  BOOST_CHECK_EQUAL(test_contraption->Get<int>("age"), 0);
  test_contraption->Set<string>("name", "Dummy");

  test_contraption->Set("age", 12);
  BOOST_CHECK_EQUAL(test_contraption->Get<int>("age"), 12);
  test_contraption->Save();

  test_backend->int_fields()[accessor.id()]["age"] = 18;

  BOOST_CHECK_EQUAL(test_contraption->Get<int>("age"), 12);
  BOOST_CHECK_EQUAL(test_contraption->Get<string>("name"),
                    string("Dummy"));
  test_contraption->Refresh();

  BOOST_CHECK_EQUAL(test_contraption->Get<int>("age"), 18);
  BOOST_CHECK_EQUAL(test_contraption->Get<string>("name"),
                    string("Dummy"));

  BOOST_CHECK(accessor.id() != Contraption::kNewID);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"],
                    18);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"],
                    string("Dummy"));
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);

  BOOST_CHECK_THROW(test_contraption->Get<int>("surname"),
                    FieldException);
}

BOOST_FIXTURE_TEST_CASE(testRefreshGetInt, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend
      = backend();
  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());

  test_contraption->Set<string>("name", "Dummy");
  test_contraption->Set("age", 12);

  test_contraption->Save();

  test_backend->int_fields()[accessor.id()]["age"] = 18;

  BOOST_CHECK_THROW(test_contraption->Get<string>(1),
                    FieldException);
  BOOST_CHECK_EQUAL(test_contraption->Get<int>(1),
                    12);
  BOOST_CHECK_EQUAL(test_contraption->Get<string>(0),
                    string("Dummy"));

  test_contraption->Refresh();

  BOOST_CHECK_EQUAL(test_contraption->Get<int>(1), 18);
  BOOST_CHECK_EQUAL(test_contraption->Get<string>(0),
                    string("Dummy"));
  BOOST_CHECK(accessor.id() != Contraption::kNewID);
  BOOST_CHECK_EQUAL(test_backend->int_fields()[accessor.id()]["age"],
                    18);
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["name"],
                    string("Dummy"));
  BOOST_CHECK_EQUAL(test_backend->int_fields().size(), 1);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(), 1);
  BOOST_CHECK_THROW(test_contraption->Get<int>(4),
                    FieldException);
}

BOOST_FIXTURE_TEST_CASE(testGetFieldIDName, Fixture) {
  ContraptionP test_contraption(
      new Contraption(model()));
  BOOST_CHECK_EQUAL(test_contraption->GetName(
      test_contraption->GetID("name")), "name");
  BOOST_CHECK_EQUAL(test_contraption->GetName(
      test_contraption->GetID("age")), "age");
  BOOST_CHECK_THROW(test_contraption->GetName(4),
                    FieldException);
  BOOST_CHECK_THROW(test_contraption->GetID("surname"),
                    FieldException);
}

BOOST_FIXTURE_TEST_CASE(testPrivate, Fixture) {
  ContraptionP test_contraption(
      new Contraption(model()));
  BOOST_CHECK_THROW(test_contraption->Get<int>("password"),
                    FieldException);
  BOOST_CHECK_THROW(test_contraption->Set("password", 1),
                    FieldException);
}

BOOST_FIXTURE_TEST_CASE(testBackendName, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend(backend());
  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());
  test_contraption->Set<string>("Surname", "Dummy");
  test_contraption->Save();
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["surname"],
                    string("Dummy"));
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()].count("Surname"),
                    0);
}

BOOST_FIXTURE_TEST_CASE(testDelete, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend(backend());
  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());
  test_contraption->Set<string>("Surname", "Dummy");
  test_contraption->Save();
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()]["surname"],
                    string("Dummy"));
  BOOST_CHECK_EQUAL(test_backend->string_fields()[accessor.id()].count("Surname"),
                    0);
  BOOST_CHECK(accessor.id() != Contraption::kNewID);
  test_contraption->Delete();
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(),
                    0);
  BOOST_CHECK_EQUAL(test_backend->string_fields().size(),
                    0);
  BOOST_CHECK_EQUAL(accessor.id(),
                    Contraption::kNewID);
}

BOOST_FIXTURE_TEST_CASE(testAll, Fixture) {
  boost::shared_ptr<MemoryModelBackend> test_backend(backend());
  ModelP test_model(new Model(fields(), test_backend));
  ContraptionP test_contraption(new Contraption(test_model));
  ContraptionAccessor accessor(test_contraption.get());
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Dummy");
  test_contraption->Save();
  test_contraption.reset(new Contraption(test_model));
  test_contraption->Set<int>("age", 12);
  test_contraption->Set<string>("Surname", "Ymmud");
  test_contraption->Save();
  ContraptionArrayP contraptions = test_model->All();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    2);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<int>("age"),
                    10);
  BOOST_CHECK_EQUAL(contraptions->at(0)->Get<string>("Surname"),
                    "Dummy");
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<int>("age"),
                    12);
  BOOST_CHECK_EQUAL(contraptions->at(1)->Get<string>("Surname"),
                    "Ymmud");
  contraptions->erase(0);
  contraptions->Save();
  contraptions = test_model->All();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    1);
  test_contraption.reset(new Contraption(test_model));
  test_contraption->Set<int>("age", 10);
  test_contraption->Set<string>("Surname", "Dummy");
  contraptions->push_back(test_contraption);
  contraptions->Save();
  contraptions = test_model->All();
  BOOST_CHECK_EQUAL(contraptions->size(),
                    2);
}

BOOST_AUTO_TEST_SUITE_END()
