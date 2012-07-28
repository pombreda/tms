#ifndef _TMS_CLIENT__OPTIONS_HPP_
#define _TMS_CLIENT__OPTIONS_HPP_

// std
#include <string>
// boost
#include <boost/filesystem.hpp>
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
#include <protocol/client.hpp>
#include "options_model.hpp"
// client
#include <client/client.hpp>

namespace tms {
namespace client {

class Options {
 public:
  static void Init();

  static std::string name() {
    return contraption_->Get<std::string>("name");
  }
  static void set_name(std::string value) {
    contraption_->Set<std::string>("name", value);
    contraption_->Save();
  }

  static std::string password_hash() {
    return contraption_->Get<std::string>("password_hash");
  }
  static void set_password_hash(std::string value) {
    contraption_->Set<std::string>("password_hash", value);
    contraption_->Save();
  }

  static std::string server() {
    return contraption_->Get<std::string>("server");
  }
  static void set_server(std::string value) {
    contraption_->Set<std::string>("server", value);
    contraption_->Save();
  }

  static std::string port() {
    return contraption_->Get<std::string>("port");
  }
  static void set_port(std::string value) {
    contraption_->Set<std::string>("port", value);
    contraption_->Save();
  }

  static common::protocol::ClientP client() {
    return client_;
  }
  static void set_client(common::protocol::ClientP client) {
    client_ = client;
  }

 private:
  static common::protocol::ClientP client_;
  static common::contraption::ContraptionP contraption_;
};

}
}

#endif // _TMS_CLIENT__OPTIONS_HPP_
