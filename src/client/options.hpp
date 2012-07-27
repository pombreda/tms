#ifndef _TMS_CLIENT__OPTIONS_HPP_
#define _TMS_CLIENT__OPTIONS_HPP_

// std
#include <string>
// soci
#include <soci/sqlite3/soci-sqlite3.h>
// common
#include <contraption/model.hpp>
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/model_backend.hpp>
#include <contraption/model_backend/soci_model_backend.hpp>
#include <contraption/field.hpp>
#include <contraption/field/simple_field.hpp>
#include "options_model.hpp"

namespace tms {
namespace client {

class Options {
 public:
  static void Init();

  static std::string name() {
    return contraption_->Get<std::string>("name");
  }
  static void set_name(std::string &value) {
    contraption_->Set<std::string>("name", value);
    contraption_->Save();
  }

  static std::string password_hash() {
    return contraption_->Get<std::string>("password_hash");
  }
  static void set_password_hash(std::string &value) {
    contraption_->Set<std::string>("password_hash", value);
    contraption_->Save();
  }

  static std::string server() {
    return contraption_->Get<std::string>("server");
  }
  static void set_server(std::string &value) {
    contraption_->Set<std::string>("server", value);
    contraption_->Save();
  }

  static std::string port() {
    return contraption_->Get<std::string>("port");
  }
  static void set_port(std::string &value) {
    contraption_->Set<std::string>("port", value);
    contraption_->Save();
  }

 private:
  static tms::common::contraption::ContraptionP contraption_;
};

}
}

#endif // _TMS_CLIENT__OPTIONS_HPP_
