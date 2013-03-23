#include "addressee.hpp"
// common
#include <contraption/field/simple_field.hpp>

namespace tms {
namespace project {
namespace model {
using namespace common::contraption;
void Addressee::Initialize() {
  vector<Field*> ret;                            
  ret.push_back(new StringField("addressee"));
  InitFields(ret);
}

}
}
}
