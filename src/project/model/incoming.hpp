#ifndef _TMS_PROJECT_MODEL__INCOMING_HPP_
#define _TMS_PROJECT_MODEL__INCOMING_HPP_
// std
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace project {
namespace model {

class Incoming : public common::contraption::ModelWrapper<Incoming> {
  friend class common::contraption::ModelWrapper<Incoming>;
  protected:
    virtual void Initialize();
    Incoming(common::contraption::ModelBackendP backend) : ModelWrapper<Incoming>(backend) {}
    Incoming(common::contraption::ModelBackend *backend) : ModelWrapper<Incoming>(backend) {}
};

}
}
}
#endif // _TMS_PROJECT_MODEL__INCOMING_HPP_
