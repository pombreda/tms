#ifndef TMS_COMMON_ARCHIVE__ARCHIVE_HPP
#define TMS_COMMON_ARCHIVE__ARCHIVE_HPP
// boost
#include <boost/filesystem.hpp>
// std
#include <vector>
namespace tms {
namespace common {
namespace archive {
  void Compress(std::vector<boost::filesystem::path> files, boost::filesystem::path archive);
  void Decompress(boost::filesystem::path archive, boost::filesystem::path destination);
}
}
}
#endif // TMS_COMMON_ARCHIVE__ARCHIVE_HPP
