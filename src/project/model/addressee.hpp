#ifndef _TMS_PROJECT_MODEL__ADDRESSEE_HPP_
#define _TMS_PROJECT_MODEL__ADDRESSEE_HPP_
// common
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace project {
namespace model {

class Addressee : public common::contraption::ModelWrapper<Addressee> {
  friend class common::contraption::ModelWrapper<Addressee>;
 protected:
  virtual void Initialize();
  Addressee(common::contraption::ModelBackendP backend) : 
      ModelWrapper<Addressee>(backend) {}
  Addressee(common::contraption::ModelBackend *backend) :
      ModelWrapper<Addressee>(backend) {}
};

}
}
}

#endif // _TMS_PROJECT_MODEL__ADDRESSEE_HPP_
