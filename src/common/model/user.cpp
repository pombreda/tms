#include "user.hpp"
// common
#include <contraption/field/simple_field.hpp>

using namespace tms::common::model;
using namespace tms::common::contraption;

User::User(ModelBackendP backend) :
    Model(fields(), backend) {}

vector<Field*> User::fields() {
  vector<Field*> ret;
  ret.push_back(new SimpleFieldT<std::string>("name"));
  ret.push_back(new SimpleFieldT<std::string>("password_hash"));
  return ret;
}

