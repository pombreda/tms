#ifndef _TMS_COMMON_RTTI__TYPEINFO_HPP_
#define _TMS_COMMON_RTTI__TYPEINFO_HPP_
#include <cxxabi.h>

// std
#include <string>
#include <typeinfo>
// boost
#include <boost/functional/hash.hpp>

namespace tms {
namespace common {
namespace rtti {
class TypeInfo;
bool operator==(const TypeInfo& a, const TypeInfo &b);
bool operator!=(const TypeInfo& a, const TypeInfo &b);
bool operator>(const TypeInfo& a, const TypeInfo &b);
bool operator<(const TypeInfo& a, const TypeInfo &b);
bool operator>=(const TypeInfo& a, const TypeInfo &b);
bool operator<=(const TypeInfo& a, const TypeInfo &b);

class TypeInfo {
public:
  TypeInfo(const std::type_info &type_info) :
      type_info_(&type_info) {}

  friend bool operator==(const TypeInfo& a, const TypeInfo &b);
  friend bool operator!=(const TypeInfo& a, const TypeInfo &b);
  friend bool operator>(const TypeInfo& a, const TypeInfo &b);
  friend bool operator<(const TypeInfo& a, const TypeInfo &b);
  friend bool operator>=(const TypeInfo& a, const TypeInfo &b);
  friend bool operator<=(const TypeInfo& a, const TypeInfo &b);
  size_t GetHash() const {
    return boost::hash_value(std::string(type_info_->name()));
  }
  std::string name() {
    int status = -4;  
    char *res = abi::__cxa_demangle(type_info_->name(), NULL, NULL, &status);
    const char* const demangled_name = (status==0)?res:type_info_->name();
    std::string ret_val(demangled_name);
    free(res);
    return ret_val;
  }
 private:
  const std::type_info *type_info_;
};

inline bool operator==(const TypeInfo& a, const TypeInfo &b) {
  return *a.type_info_ == *b.type_info_;
}

inline bool operator!=(const TypeInfo& a, const TypeInfo &b) {
  return *a.type_info_ != *b.type_info_;
}

inline bool operator>(const TypeInfo& a, const TypeInfo &b) {
  return b.type_info_->before(*a.type_info_);
}

inline bool operator<(const TypeInfo& a, const TypeInfo &b) {
  return a.type_info_->before(*b.type_info_);
}

inline bool operator>=(const TypeInfo& a, const TypeInfo &b) {
  return !(a < b);
}

inline bool operator<=(const TypeInfo& a, const TypeInfo &b) {
  return !(a > b);
}

inline size_t hash_value(const TypeInfo &ti) {
  return ti.GetHash();
}

template<class T>
TypeInfo TypeID(const T &object) {
  return TypeInfo(typeid(object));
}

template<class T>
TypeInfo TypeID() {
  return TypeInfo(typeid(T));
}

}
}
}
#endif // _TMS_COMMON_RTTI__TYPEINFO_HPP_
