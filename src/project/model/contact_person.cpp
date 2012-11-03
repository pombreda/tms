#include "contact_person.hpp"
// common
#include <contraption/field/simple_field.hpp>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void ContactPerson::Initialize() {
  vector<Field*> ret;
  ret.push_back(new SimpleFieldT<string>("name"));
  ret.push_back(new SimpleFieldT<string>("code"));
  ret.push_back(new SimpleFieldT<string>("email"));
  ret.push_back(new SimpleFieldT<string>("note"));
  ret.push_back(new SimpleFieldT<string>("phone"));
  ret.push_back(new SimpleFieldT<string>("fax"));
  InitFields(ret);
}

