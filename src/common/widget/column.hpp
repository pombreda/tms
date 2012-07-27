#ifndef _TMS_COMMON_WIDGET__COLUMN_HPP_
#define _TMS_COMMON_WIDGET__COLUMN_HPP_

#include <cstdio>

struct Column {
  size_t field_id;
  std::string name;
  size_t width;
  Column(size_t field_id, std::string name, size_t width) :
    field_id(field_id), name(name), width(width) {}
};

#endif // _TMS_COMMON_WIDGET__COLUMN_HPP_
