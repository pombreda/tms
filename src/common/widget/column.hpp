#ifndef _TMS_COMMON_WIDGET__COLUMN_HPP_
#define _TMS_COMMON_WIDGET__COLUMN_HPP_

#include <string>

struct Column {
  int field_id;
  std::string name;
  int width;
  Column(int field_id, std::string name, int width) :
    field_id(field_id), name(name), width(width) {}
};

#endif // _TMS_COMMON_WIDGET__COLUMN_HPP_
