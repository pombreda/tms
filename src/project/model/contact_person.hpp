#ifndef _TMS_PROJECT_MODEL__CONTACT_PERSON_HPP_
#define _TMS_PROJECT_MODEL__CONTACT_PERSON_HPP_
// std 
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model.hpp>
#include <contraption/model_backend.hpp>

namespace tms {
namespace project {
namespace model {

class ContactPerson : public common::contraption::Model {
 public:
  ContactPerson(common::contraption::ModelBackendP backend);
 private:
  static std::vector<common::contraption::Field*> fields();
};

}
}
}
#endif // _TMS_PROJECT_MODEL__CONTACT_PERSON_HPP_
