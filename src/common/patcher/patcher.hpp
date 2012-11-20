#ifndef TMS_COMMON_PATCHER__PATCHER_HPP
#define TMS_COMMON_PATCHER__PATCHER_HPP
// boost
#include <boost/filesystem.hpp>
namespace tms {
namespace common {
namespace patcher {
  void Patch(boost::filesystem::path patch, boost::filesystem::path root);
}
}
}

#endif // TMS_COMMON_PATCHER__PATCHER_HPP
