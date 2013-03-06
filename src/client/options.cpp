#include <cstdarg> // for log4cplus
#include <sstream>
#include <string/string.hpp>
#include "options.hpp"
// log4cplus
#include <client/logger.hpp>
#include <log4cplus/loggingmacros.h>


namespace tms {
namespace client {

using namespace tms::common::contraption;
using namespace tms::common::protocol;
using namespace tms::common::string;

ContraptionP Options::contraption_ = ContraptionP();
ClientP Options::client_ = ClientP();
bool Options::admin_ = false;
bool Options::secretary_ = false;
bool Options::manager_ = false;
map<std::string, ColumnLayout> Options::incoming_columns_ = map<std::string, ColumnLayout>();

void Options::Save() {
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Saving options"));
  
  std::ostringstream incoming_layout_stream(contraption_->Get<std::string>("incoming_column_layout"));
  for (map<std::string, ColumnLayout>::iterator it = incoming_columns_.begin();
       it != incoming_columns_.end(); ++it) {
    incoming_layout_stream << it->first <<
        " => " << it->second.width <<
        " " << it->second.pos <<
        " " << it->second.enabled << endl;
  }
  contraption_->Set<std::string>("incoming_column_layout", incoming_layout_stream.str());
  contraption_->Save();
  LOG4CPLUS_INFO(client_logger, 
                 WStringFromUTF8String("Options saved"));    
}

void Options::Init() {
  LOG4CPLUS_INFO(client_logger, 
                   WStringFromUTF8String("Loading options"));
    
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
  } else {
    contraption_ = contraptions->at(0);
    contraption_->Get<std::string>("incoming_column_layout");
    std::istringstream incoming_layout_stream(contraption_->Get<std::string>("incoming_column_layout"));
    std::string token;
    std::string column_name;
    incoming_layout_stream >> token;
    column_name = token;
    while (!incoming_layout_stream.eof() && !incoming_layout_stream.fail()) {
      for (incoming_layout_stream >> token; token != "=>"; incoming_layout_stream >> token) {
        column_name = column_name + " " + token;
      }
      ColumnLayout column_layout;
      incoming_layout_stream >> column_layout.width >> column_layout.pos
                    >> column_layout.enabled;
      incoming_columns_[column_name] = column_layout;
      incoming_layout_stream >> column_name;
    }
  }

  LOG4CPLUS_INFO(client_logger, 
                   WStringFromUTF8String("Options loaded"));    
}

}
}
