#include <cstdarg> // for log4cplus
#include "options.hpp"

namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::protocol;

ContraptionP Options::contraption_ = ContraptionP();
ClientP Options::client_ = ClientP();
bool Options::admin_ = false;
bool Options::secretair_ = false;
bool Options::manager_ = false;

void Options::Init() {
  std::string options_db("options.sqlite3");
  bool need_initialization = false;
  if (!boost::filesystem::exists(options_db)) {
    need_initialization = true;
  }
  SOCIDBScheme scheme(soci::sqlite3, options_db);
  ModelBackendP backend(new SOCIModelBackend(scheme, "options"));
  ModelP model(new OptionsModel(backend));
  if (need_initialization) {
    model->InitSchema();
  }
  ContraptionArrayP contraptions = model->All();
  if (contraptions->size() == 0) {
    contraption_ = model->New();
    contraption_->Set<std::string>("name", "admin");
    contraption_->Set<std::string>("password_hash", "");
    contraption_->Set<std::string>("server", "127.0.0.1");
    contraption_->Set<std::string>("port", "3300");
    contraption_->Save();
  } else {
    contraption_ = contraptions->at(0);
  }
}

}
}
