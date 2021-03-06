#ifndef _TMS_PROJECT_MODEL__COMPANY_HPP_
#define _TMS_PROJECT_MODEL__COMPANY_HPP_
// common
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace project {
namespace model {

class Company : public common::contraption::ModelWrapper<Company> {
  friend class common::contraption::ModelWrapper<Company>;
 public:
  static std::string GenerateID();
 protected:
  virtual void Initialize();
  Company(common::contraption::ModelBackendP backend) :
      ModelWrapper<Company>(backend) {}
  Company(common::contraption::ModelBackend *backend) :
      ModelWrapper<Company>(backend) {}
};

}
}
}
#endif // _TMS_PROJECT_MODEL__COMPANY_HPP_
