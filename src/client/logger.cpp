#include "logger.hpp"
#include <log4cplus/configurator.h>
// common
#include <string/string.hpp>
using namespace log4cplus;
using namespace tms::common::string;

Logger
client_logger = Logger
  ::getInstance(WStringFromUTF8String("client::main"));

namespace {
class Configurator {
public:
  Configurator() {
    PropertyConfigurator config(WStringFromUTF8String("log.cfg"));
    config.configure();
  }
} dummy;
}

