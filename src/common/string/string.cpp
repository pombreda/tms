#include "string.hpp"
// std
#include <vector>
// iconv
#include <iconv.h>

using namespace tms::common::string;
using namespace std;

std::string tms::common::string::UTF8StingFromWString(std::wstring str) {
  iconv_t cd = iconv_open("UTF-8", "WCHAP_T");
  size_t s1 = str.size() * sizeof(wchar_t);
  size_t s2 = s1 * 2;
  vector<char> str2(s2);
  char *obuf = str2.data();
  const wchar_t *ibuf = str.data();
 
  size_t rc = iconv(cd, (const char**)&ibuf, &s1, &obuf, &s2);  
  if (static_cast<int>(rc) < 0) return "";
  return std::string(str2.begin(), str2.begin() + rc );
}

std::wstring tms::common::string::WStringFromUTF8String(std::string str) {
  iconv_t cd = iconv_open("WCHAR_T", "UTF-8");
  size_t s1 = str.size();
  size_t s2 = s1 * sizeof(wchar_t);
  vector<wchar_t> str2(s1);
  wchar_t *obuf = str2.data();
  const char *ibuf = str.data();
 
  size_t rc = iconv(cd, &ibuf, &s1, (char **)&obuf, &s2);  
  if (static_cast<int>(rc) < 0) return std::wstring();
  return std::wstring(str2.begin(), str2.begin() + rc );
}
