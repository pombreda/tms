#ifndef _TMS_PROJECT_MODEL__SUBJECT_HPP_
#define _TMS_PROJECT_MODEL__SUBJECT_HPP_
// common
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace project {
namespace model {

class Subject : public common::contraption::ModelWrapper<Subject> {
  friend class common::contraption::ModelWrapper<Subject>;
 protected:
  virtual void Initialize();
  Subject(common::contraption::ModelBackendP backend) : 
      ModelWrapper<Subject>(backend) {}
  Subject(common::contraption::ModelBackend *backend) :
      ModelWrapper<Subject>(backend) {}
};

}
}
}

#endif // _TMS_PROJECT_MODEL__SUBJECT_HPP_
