#include "options_model.hpp"
// common
#include <contraption/field/simple_field.hpp>

namespace tms {
namespace client {

using namespace tms::common::contraption;

OptionsModel::OptionsModel(ModelBackendP backend) :
    Model(fields(), backend) {}

std::vector<Field*> OptionsModel::fields() {
  std::vector<Field*> ret;
  ret.push_back(new SimpleFieldT<std::string>("name"));
  ret.push_back(new SimpleFieldT<std::string>("password_hash"));
  ret.push_back(new SimpleFieldT<std::string>("server"));
  ret.push_back(new SimpleFieldT<std::string>("port"));
  ret.push_back(new SimpleFieldT<std::string>("column_layout"));
  return ret;
}

}
}
