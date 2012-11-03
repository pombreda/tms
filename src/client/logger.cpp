#include "logger.hpp"
#include <log4cplus/configurator.h>
// common
#include <string/string.hpp>
log4cplus::Logger
client_logger = log4cplus::Logger
  ::getInstance(LOG4CPLUS_TEXT("client::main"));
using namespace log4cplus;
using namespace tms::common::string;

namespace {
class Configurator {
public:
  Configurator() {
    PropertyConfigurator config(WStringFromUTF8String("log.cfg"));
    config.configure();
  }
} dummy;
}

