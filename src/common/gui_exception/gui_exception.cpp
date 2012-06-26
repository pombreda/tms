#include "gui_exception.hpp"
using namespace tms::common;
using namespace std;

void GUIException::Init(const std::string &message, const std::string &prev_message) {
ostringstream ret;
  if (message.size() > 0) {
    ret << message;
    if (prev_message.size() > 0) {
      ret << endl << "==" << endl;
    }
  }
  ret << prev_message;
  message_ = ret.str();
}
