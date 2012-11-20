
#include "incoming.hpp"
// common
#include <contraption/field/simple_field.hpp>
#include <contraption/field/proxy_field.hpp>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void Incoming::Initialize() {
  vector<Field*> ret;
  ret.push_back(new SimpleFieldT<string>("ID"));
  ret.push_back(new SimpleFieldT<string>("time_in"));
  ret.push_back(new SimpleFieldT<string>("type_in"));
  ret.push_back(new SimpleFieldT<int>("contact_person_id"));
  ret.push_back(new SimpleFieldT<string>("time_out"));
  InitFields(ret);
}

