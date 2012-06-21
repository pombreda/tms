#include <exception/guiexception.hpp>
#include <FL/Fl.H>
using namespace tms::common;

void Report() {
  try {
    GUIException a(std::string("error"));
    throw a;
  } catch (GUIException e) {
    e.Report();
  }
}


int main(int argc, char **argv) {
  Fl::scheme(NULL);
  Fl::args(argc, argv);
  Fl::get_system_colors();
  Report();
  return 0;
}

