#ifndef _TMS_CLIENT__OPTIONS_MODEL_HPP_
#define _TMS_CLIENT__OPTIONS_MODEL_HPP_
// std
#include <vector>
// boost
#include <boost/unordered_map.hpp>
// commone
#include <contraption/model.hpp>
#include <contraption/model_backend.hpp>

namespace tms {
namespace client {

class OptionsModel : public tms::common::contraption::Model {
 public:
  OptionsModel(tms::common::contraption::ModelBackendP backend);
 private:
  static std::vector<tms::common::contraption::Field*> fields();
};

}
}

#endif // _TMS_CLIENT__OPTIONS_MODEL_HPP_

