#ifndef _TMS_COMMON_MODEL__USER_HPP_
#define _TMS_COMMON_MODEL__USER_HPP_
// std 
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model.hpp>
#include <contraption/model_backend.hpp>

namespace tms {
namespace common {
namespace model {

class User : public contraption::Model {
 public:
  User(contraption::ModelBackendP backend);
 private:
  static std::vector<contraption::Field*> fields();
};

}
}
}
#endif // _TMS_COMMON_MODEL__USER_HPP_
