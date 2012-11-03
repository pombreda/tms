#ifndef _TMS_PROJECT_MODEL__CONTACT_PERSON_HPP_
#define _TMS_PROJECT_MODEL__CONTACT_PERSON_HPP_
// std
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model_wrapper.hpp>

namespace tms {
namespace project {
namespace model {

class ContactPerson : public common::contraption::ModelWrapper<ContactPerson> {
  friend class common::contraption::ModelWrapper<ContactPerson>;
  protected:
    virtual void Initialize();
    ContactPerson(common::contraption::ModelBackendP backend) : ModelWrapper<ContactPerson>(backend) {}
    ContactPerson(common::contraption::ModelBackend *backend) : ModelWrapper<ContactPerson>(backend) {}
};

}
}
}
#endif // _TMS_PROJECT_MODEL__CONTACT_PERSON_HPP_
