
#include "scheme.hpp"
// common
#include <contraption/field/simple_field.hpp>
using namespace tms::project::model;
using namespace tms::common::contraption;
using namespace std;

void Scheme::Initialize() {
  vector<Field*> ret;
  ret.push_back(new SimpleFieldT<std::string>("version"));
  InitFields(ret);
}

