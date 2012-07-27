#ifndef _TMS_COMMON_PROTOCOL__CRYPTO_HPP_
#define _TMS_COMMON_PROTOCOL__CRYPTO_HPP_

#include <string>

namespace tms {
namespace common {
namespace protocol {

std::string sha256(const std::string &str);

}
}
}

#endif // _TMS_COMMON_PROTOCOL__CRYPTO_HPP_
