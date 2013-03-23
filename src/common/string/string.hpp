#ifndef TMS_COMMON__STRING_HPP
#define TMS_COMMON__STRING_HPP
#include <string>

namespace tms {
namespace common {
namespace string {

  std::string UTF8StringFromWString(std::wstring str);

  std::wstring WStringFromUTF8String(std::string str);

  std::wstring WStringFromCP1251String(std::string str);

}
}
}
#endif // TMS_COMMON__STRING_HPP
