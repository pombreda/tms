#include "string.hpp"
// std
#include <vector>
#include <iostream>
#include <cassert>
// iconv
#include <iconv.h>
#include <errno.h>
#include <gui_exception/gui_exception.hpp>

using namespace tms::common::string;
using namespace std;
using namespace tms::common;
class ConvertException : public GUIException {
 public:
  explicit ConvertException(const std::string &message) throw () : 
      GUIException(message) { }
  
  explicit ConvertException(const std::exception *std_exception, const std::string &message = "") throw () :
      GUIException(std_exception, message) { }   
  
  ConvertException(const GUIException *gui_exception, const std::string &message) throw () : 
      GUIException(gui_exception, message) {}

};

std::string tms::common::string::UTF8StringFromWString(std::wstring str) {
  iconv_t cd = iconv_open("UTF-8", "WCHAR_T");
  size_t s1 = str.size() * sizeof(wchar_t);
  size_t s2 = s1 * 4;
  vector<char> str2(s2);
  char *obuf = str2.data();
  char *obufs = obuf;
  const char *ibuf = reinterpret_cast<const char*>(str.data());
 
  if (!obuf) {
    return "";
  }
  size_t rc = iconv(cd, const_cast<char**>(&ibuf), &s1, &obuf, &s2);  
  if (static_cast<int>(rc) < 0) {
    throw ConvertException("Convertion error");
  }
  return std::string(obufs, obuf);
}


std::wstring tms::common::string::WStringFromUTF8String(std::string str) {
  iconv_t cd = iconv_open("WCHAR_T", "UTF-8");
  size_t s1 = str.size();
  size_t s2 = s1 * sizeof(wchar_t);
  vector<wchar_t> str2(s1);
  wchar_t *obuf = str2.data();
  wchar_t *obufs = obuf;
  const char *ibuf = str.data();
  size_t rc = iconv(cd, const_cast<char**>(&ibuf), &s1, reinterpret_cast<char**>(&obuf), &s2);  
  if (!obuf) {
    return L"";
  }
  if (static_cast<int>(rc) < 0) {
    throw ConvertException("Convertion error");
  }
  return std::wstring(obufs, obuf);
}

std::wstring tms::common::string::WStringFromCP1251String(std::string str) {
  iconv_t cd = iconv_open("WCHAR_T", "CP1251");
  size_t s1 = str.size();
  size_t s2 = s1 * sizeof(wchar_t);
  vector<wchar_t> str2(s1);
  wchar_t *obuf = str2.data();
  wchar_t *obufs = obuf;
  const char *ibuf = str.data();
  size_t rc = iconv(cd, const_cast<char**>(&ibuf), &s1, reinterpret_cast<char**>(&obuf), &s2);
  if (!obuf) {
    return L"";
  }
  if (static_cast<int>(rc) < 0) {
    throw ConvertException("Convertion error");
  }
  return std::wstring(obufs, obuf);
}

