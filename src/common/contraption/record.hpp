#ifndef _TMS_COMMON_CONTRAPTION__RECORD_HPP_
#define _TMS_COMMON_CONTRAPTION__RECORD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <string>
//
#include "record_fwd.hpp"


namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// Record class.
//------------------------------------------------------------
class Record {
 public:
  Record(const std::string &field) :
      field(field) {}
  virtual ~Record() {}
  const std::string &field;
};

//------------------------------------------------------------
// RecordT class.
//------------------------------------------------------------
template<typename T>
class RecordT : public Record {
 public:
  RecordT(const std::string &field, T *data) :
      Record(field),
      data(data) {}
  T *data;
 private:
  RecordT(const RecordT<T> &other);
  RecordT<T>& operator=(const RecordT<T> &other);
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__RECORD_HPP_
