#ifndef _TMS_COMMON_MODEL__USER_HPP_
#define _TMS_COMMON_MODEL__USER_HPP_
// std
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace common {
namespace model {

class User : public contraption::ModelWrapper<User> {
  friend class contraption::ModelWrapper<User>;
  protected:
    virtual void Initialize();
    User(contraption::ModelBackendP backend) : ModelWrapper<User>(backend) {}
    User(contraption::ModelBackend *backend) : ModelWrapper<User>(backend) {}
};

}
}
}
#endif // _TMS_COMMON_MODEL__USER_HPP_
