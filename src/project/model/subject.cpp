#include "subject.hpp"
// common
#include <contraption/field/simple_field.hpp>

namespace tms {
namespace project {
namespace model {
using namespace common::contraption;
void Subject::Initialize() {
  vector<Field*> ret;                            
  ret.push_back(new StringField("subject"));
  InitFields(ret);
}

}
}
}
