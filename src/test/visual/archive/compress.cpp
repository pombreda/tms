// common
#include <archive/archive.hpp>
// boost
#include <boost/filesystem.hpp>

using namespace tms::common::archive;
using namespace boost::filesystem;
using namespace std;

int main() {
  vector<path> files;
  files.push_back(path("test.txt"));
  files.push_back(path("test_folder"));
  files.push_back(path("test_folder/test2.txt"));
  path archive("test.tar.gz");
  Compress(files, archive);
}

