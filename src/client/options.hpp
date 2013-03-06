
#ifndef _TMS_CLIENT__OPTIONS_HPP_
#define _TMS_CLIENT__OPTIONS_HPP_

// std
#include <string>
#include <map>
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
#include <client/dlg_login.hpp>

namespace tms {
namespace client {

struct ColumnLayout {
  int width;
  int pos;
  bool enabled;
  ColumnLayout() :
      width(-1),
      pos(-1),
      enabled(true) {}
};

class Options {
 public:
  static void Init();

  static std::string name() {
    return contraption_->Get<std::string>("name");
  }
  static void set_name(std::string value) {
    contraption_->Set<std::string>("name", value);
  }
  static void set_admin(bool admin) {
    admin_ = admin;
  }
  static void set_manager(bool manager) {
    manager_ = manager;
  }
  static void set_secretary(bool secretary) {
    secretary_ = secretary;
  }


  static std::string password_hash() {
    return contraption_->Get<std::string>("password_hash");
  }
  static void set_password_hash(std::string value) {
    contraption_->Set<std::string>("password_hash", value);
  }

  static bool admin() {
    return admin_;
  }
  static bool manager() {
    return manager_;
  }
  static bool secretary() {
    return secretary_;
  }
  static std::string server() {
    return contraption_->Get<std::string>("server");
  }
  static void set_server(std::string value) {
    contraption_->Set<std::string>("server", value);
  }

  static std::string port() {
    return contraption_->Get<std::string>("port");
  }
  static void set_port(std::string value) {
    contraption_->Set<std::string>("port", value);
  }

  static common::protocol::ClientP client() {
    return client_;
  }
  static void set_client(common::protocol::ClientP client) {
    client_ = client;
  }

  static ColumnLayout incoming_column_layout(std::string column) {
    return incoming_columns_[column];
  }

  static void set_incoming_column_layout(std::string column, ColumnLayout layout) {
    incoming_columns_[column] = layout;
  }

  static void Save();

 private:
  static common::protocol::ClientP client_;
  static common::contraption::ContraptionP contraption_;
  static bool admin_;
  static bool manager_;
  static bool secretary_;
  static std::map<std::string, ColumnLayout> incoming_columns_;
};

}
}

#endif // _TMS_CLIENT__OPTIONS_HPP_
