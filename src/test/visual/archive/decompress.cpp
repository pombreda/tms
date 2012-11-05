// common
#include <archive/archive.hpp>
// boost
#include <boost/filesystem.hpp>

using namespace tms::common::archive;
using namespace boost::filesystem;
using namespace std;

int main() {
  path archive("test.tar.gz");
  path ext_dir("extracted");
  Decompress(archive, ext_dir);
}
