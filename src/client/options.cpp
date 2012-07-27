#include "options.hpp"

namespace tms {
namespace client {

using namespace tms::common::contraption;

ContraptionP Options::contraption_ = ContraptionP();

void Options::Init() {
  std::string options_db("options.sqlite3");
  SOCIDBScheme scheme(soci::sqlite3, options_db);
  ModelBackendP backend(new SOCIModelBackend(scheme, "options"));
  ModelP model(new OptionsModel(backend));
  model->InitSchema();
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
