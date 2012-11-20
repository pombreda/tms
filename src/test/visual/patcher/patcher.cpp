// common
#include <patcher/patcher.hpp>
// boost
#include <boost/filesystem.hpp>

using namespace tms::common::patcher;
using namespace boost::filesystem;

int main() {
  Patch(path("test.patch"), current_path());
  return 0;
}
