#ifndef _TMS_PROJECT_MODEL__SCHEME_HPP_
#define _TMS_PROJECT_MODEL__SCHEME_HPP_
// std
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace project {
namespace model {

class Scheme : public common::contraption::ModelWrapper<Scheme> {
  friend class common::contraption::ModelWrapper<Scheme>;
  protected:
    virtual void Initialize();
    Scheme(common::contraption::ModelBackendP backend) : ModelWrapper<Scheme>(backend) {}
    Scheme(common::contraption::ModelBackend *backend) : ModelWrapper<Scheme>(backend) {}
};

}
}
}
#endif // _TMS_PROJECT_MODEL__SCHEME_HPP_
